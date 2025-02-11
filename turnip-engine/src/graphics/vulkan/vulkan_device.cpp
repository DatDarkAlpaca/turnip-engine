#include "pch.hpp"
#include "vulkan_device.hpp"
#include "vulkan_command_buffer.hpp"
#include "platform/vulkan_context.hpp"

#include "builders/instance_builder.hpp"
#include "builders/physical_device_builder.hpp"
#include "builders/logical_device_builder.hpp"
#include "builders/vma_allocator_builder.hpp"
#include "builders/swapchain_builder.hpp"
#include "builders/frame_data_builder.hpp"

#include "factories/shader_factory.hpp"
#include "factories/pipeline_factory.hpp"
#include "factories/buffer_factory.hpp"
#include "factories/texture_factory.hpp"

#include "platform/platform.hpp"

namespace tur::vulkan
{
	void GraphicsDeviceVulkan::initialize_impl(NON_OWNING Window* window, const ConfigData& configData)
	{
		const auto& vulkanConfig = configData.vulkanConfiguration;
		r_Window = window;
		m_ConfigData = configData;

		// Instance:
		initialize_instance(m_State, configData);

		// Surface:
		m_State.surface = get_vulkan_surface(m_State.instance, window);

		// Physical Device:
		initialize_physical_device(m_State, vulkanConfig.physicalDeviceRequirements);

		// Logical Device:
		initialize_logical_device(m_State, vulkanConfig);
	
		// VMA Allocator:
		initialize_vma_allocator(m_State);

		// Swapchain:
		initialize_swapchain(m_State, {});

		// Frame Data:
		initialize_frame_data(m_State);

		// Texture:
		TextureDescriptor descriptor;
		{
			descriptor.format = TextureFormat::RGBA16SFloat;
			descriptor.width = m_State.swapchainExtent.width;
			descriptor.height = m_State.swapchainExtent.height;
			m_State.drawTexture = m_Textures.get(create_texture(descriptor, {}));
		}

		// Immediate command buffer:
		{			
			vk::CommandPoolCreateInfo poolCreateInfo = {};
			{
				poolCreateInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
				poolCreateInfo.queueFamilyIndex = m_State.queueList.get_family_index(QueueUsage::TRANSFER);
			}

			try
			{
				m_ImmCommandPool = m_State.logicalDevice.createCommandPool(poolCreateInfo);
			}
			catch (vk::SystemError& err)
			{
				TUR_LOG_CRITICAL("Failed to create immediate command pool. {}", err.what());
			}

			vk::CommandBufferAllocateInfo allocateInfo = {};
			{
				allocateInfo.commandPool = m_ImmCommandPool;
				allocateInfo.commandBufferCount = 1;
				allocateInfo.level = vk::CommandBufferLevel::ePrimary;
			}

			m_ImmCommandBuffer = m_State.logicalDevice.allocateCommandBuffers(allocateInfo).front();

			{
				vk::FenceCreateInfo createInfo = {};
				createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
				m_ImmFence = m_State.logicalDevice.createFence(createInfo);
			}
		}
	}
	void GraphicsDeviceVulkan::present_impl()
	{
		auto& graphicsQueue = m_State.queueList.get(QueueUsage::GRAPHICS);
		auto& presentQueue = m_State.queueList.get(QueueUsage::PRESENT);
		auto& frameDataHolder = m_State.frameDataHolder;
		const auto& frameData = frameDataHolder.get_frame_data();

		vk::Semaphore signalSemaphores[] = { frameData.renderFinishedSemaphore };
		vk::Semaphore waitSemaphores[] = { frameData.imageAvailableSemaphore };

		u32 imageIndices[] = { frameDataHolder.get_color_buffer() };
		vk::PresentInfoKHR presentInfo = {};
		{
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &m_State.swapchain;
			presentInfo.pImageIndices = imageIndices;
		}

		bool& framebufferResized = r_Window->data.framebufferResized;

		vk::Result presentResult = {};
		try
		{
			presentResult = presentQueue.presentKHR(presentInfo);
		}
		catch (vk::SystemError& err)
		{
			if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || framebufferResized)
			{
				framebufferResized = false;
				recreate_swapchain();
			}
			else if (presentResult != vk::Result::eSuccess)
				TUR_LOG_CRITICAL("Failed to acquire swapchain image. {}", err.what());
		}

		frameDataHolder.increment_frame_count();
	}

	CommandBufferVulkan GraphicsDeviceVulkan::create_command_buffer_impl()
	{
		return CommandBufferVulkan(this);
	}
	void GraphicsDeviceVulkan::initialize_gui_graphics_system_impl()
	{
	}

	buffer_handle GraphicsDeviceVulkan::create_default_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data)
	{	
		// Source Buffer:
		BufferDescriptor srcDescriptor = {};
		{
			srcDescriptor.usage = descriptor.usage;
			srcDescriptor.type = descriptor.type | BufferType::TRANSFER_DST;
			srcDescriptor.memoryUsage = BufferMemoryUsage::GPU_ONLY;
		}
		Buffer buffer = vulkan::create_buffer(m_State.vmaAllocator, srcDescriptor, data.size);
		
		// Staging buffer:
		BufferDescriptor stagingDescriptor = {};
		{
			stagingDescriptor.memoryUsage = BufferMemoryUsage::CPU_ONLY;
			stagingDescriptor.type = BufferType::TRANSFER_SRC;
		}
		Buffer stagingBuffer = vulkan::create_buffer(m_State.vmaAllocator, stagingDescriptor, data.size);

		// Buffer data:
		if(data.data)
		{
			void* bufferData = nullptr;
			vmaMapMemory(m_State.vmaAllocator, stagingBuffer.allocation, &bufferData);

			std::memcpy(bufferData, (u8*)data.data, data.size);

			vmaUnmapMemory(m_State.vmaAllocator, stagingBuffer.allocation);
		}

		// Buffer copy:
		submit_immediate_command([&]() {
			vk::BufferCopy region;
			{
				region.dstOffset = 0;
				region.srcOffset = 0;
				region.size = data.size;
			}

			m_ImmCommandBuffer.copyBuffer(stagingBuffer.buffer, buffer.buffer, region);
		});

		vmaDestroyBuffer(m_State.vmaAllocator, stagingBuffer.buffer, stagingBuffer.allocation);
		return m_Buffers.add(buffer);
	}
	buffer_handle GraphicsDeviceVulkan::create_buffer_impl(const BufferDescriptor& descriptor, u32 size)
	{
		return m_Buffers.add(vulkan::create_buffer(m_State.vmaAllocator, descriptor, size));
	}
	void GraphicsDeviceVulkan::update_buffer_impl(buffer_handle handle, const DataBuffer& data, u32 offset)
	{
		Buffer& buffer = m_Buffers.get(handle);
		void* bufferData = nullptr;

		vmaMapMemory(m_State.vmaAllocator, buffer.allocation, &bufferData);
		std::memcpy(bufferData, (u8*)data.data + offset, data.size);
		vmaUnmapMemory(m_State.vmaAllocator, buffer.allocation);
	}
	void GraphicsDeviceVulkan::copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset, u32 dstOffset)
	{
		vk::BufferCopy region;
		{
			region.dstOffset = dstOffset;
			region.srcOffset = srcOffset;
			region.size = size;
		}

		Buffer& srcBuffer = m_Buffers.get(source);
		Buffer& dstBuffer = m_Buffers.get(destination);

		m_ImmCommandBuffer.copyBuffer(srcBuffer.buffer, dstBuffer.buffer, region);
	}
	void GraphicsDeviceVulkan::destroy_buffer_impl(buffer_handle handle)
	{
		Buffer& buffer = m_Buffers.get(handle);
		vmaDestroyBuffer(m_State.vmaAllocator, buffer.buffer, buffer.allocation);

		m_Buffers.remove(handle);
	}

	texture_handle GraphicsDeviceVulkan::create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset)
	{
		return m_Textures.add(vulkan::create_texture(m_State.vmaAllocator, m_State.logicalDevice, descriptor));
	}

	shader_handle GraphicsDeviceVulkan::create_shader_impl(const ShaderDescriptor& descriptor)
	{
		vk::ShaderModule shaderModule = create_shader_module(m_State.logicalDevice, descriptor);
		return m_ShaderModules.add(shaderModule);
	}
	void GraphicsDeviceVulkan::destroy_shader_impl(shader_handle handle)
	{
		auto shaderModule = m_ShaderModules.get(handle);
		m_State.logicalDevice.destroyShaderModule(shaderModule);
		m_ShaderModules.remove(handle);
	}

	pipeline_handle GraphicsDeviceVulkan::create_graphics_pipeline_impl(const PipelineDescriptor& descriptor)
	{
		vk::Pipeline pipeline = vulkan::create_graphics_pipeline(*this, descriptor);
	
		destroy_shader(descriptor.vertexShader);

		if (descriptor.tesselationControlShader != invalid_handle)
			destroy_shader(descriptor.tesselationControlShader);

		if (descriptor.tesselationEvaluationShader != invalid_handle)
			destroy_shader(descriptor.tesselationEvaluationShader);

		if (descriptor.geometryShader != invalid_handle)
			destroy_shader(descriptor.geometryShader);

		destroy_shader(descriptor.fragmentShader);

		return m_Pipelines.add({ pipeline, PipelineType::GRAPHICS });
	}
	
	void GraphicsDeviceVulkan::recreate_swapchain()
	{
		auto size = get_window_size(r_Window);

		while (size.x == 0 || size.y == 0)
			size = get_window_size(r_Window);

		m_State.logicalDevice.waitIdle();
		cleanup_swapchain(m_State);
		
		// New Swapchain:
		SwapchainRequirements requirements;
		requirements.oldSwapchain = m_State.swapchain;
		initialize_swapchain(m_State, requirements);

		initialize_frame_data(m_State);
	}

	void GraphicsDeviceVulkan::submit_immediate_command(std::function<void()>&& function)
	{
		m_State.logicalDevice.resetFences(m_ImmFence);
		m_ImmCommandBuffer.reset();

		vk::CommandBufferBeginInfo beginInfo = {};
		beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

		m_ImmCommandBuffer.begin(beginInfo);
		
		function();

		m_ImmCommandBuffer.end();

		vk::CommandBufferSubmitInfo submitCommandInfo = {};
		{
			submitCommandInfo.commandBuffer = m_ImmCommandBuffer;
			submitCommandInfo.deviceMask = 0;
		}
		
		vk::SubmitInfo2 submitInfo = {};
		{
			submitInfo.flags = vk::SubmitFlags::Flags();

			submitInfo.signalSemaphoreInfoCount = 0;
			submitInfo.pSignalSemaphoreInfos = nullptr;

			submitInfo.waitSemaphoreInfoCount = 0;
			submitInfo.pWaitSemaphoreInfos = nullptr;

			submitInfo.commandBufferInfoCount = 1;
			submitInfo.pCommandBufferInfos = &submitCommandInfo;
		}

		m_State.queueList.get(QueueUsage::TRANSFER).submit2(submitInfo, m_ImmFence);
		m_State.logicalDevice.waitForFences(m_ImmFence, true, 1000000000);
	}
}