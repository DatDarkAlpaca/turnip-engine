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

#include "factories/pipeline_factory.hpp"
#include "factories/shader_factory.hpp"

namespace tur::vulkan
{
	void GraphicsDeviceVulkan::initialize_impl(NON_OWNING Window* window, const ConfigData& configData)
	{
		const auto& vulkanConfig = configData.vulkanConfiguration;

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
	}
	void GraphicsDeviceVulkan::present_impl()
	{
		auto& graphicsQueue = m_State.queueList.get(QueueUsage::GRAPHICS);
		auto& presentQueue = m_State.queueList.get(QueueUsage::PRESENT);
		auto& frameDataHolder = m_State.frameDataHolder;
		const auto& frameData = frameDataHolder.get_frame_data();

		// Submit:
		vk::Semaphore signalSemaphores[] = { frameData.renderFinishedSemaphore };
		vk::Semaphore waitSemaphores[] = { frameData.imageAvailableSemaphore };
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		vk::SubmitInfo submitInfo = {};
		{
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;

			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &frameData.commandBuffer;
		}

		try
		{
			graphicsQueue.submit(submitInfo, frameData.recordingFence);
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to submit commands to the graphics queue. {}", err.what());
		}

		// Present:
		u32 imageIndices[] = { frameDataHolder.get_color_buffer() };
		vk::PresentInfoKHR presentInfo = {};
		{
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &m_State.swapchain;
			presentInfo.pImageIndices = imageIndices;
		}

		try
		{
			auto _ = presentQueue.presentKHR(presentInfo);
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to submit a present command to the present queue. {}", err.what());
		}

		frameDataHolder.increment_frame_count();

		// Acquire next image:
		/*m_State.frameDataHolder.increment_frame_count();
		m_State.logicalDevice.waitForFences(frameData.recordingFence, true, 1'000'000'000);
		m_State.logicalDevice.resetFences(frameData.recordingFence);

		auto image = m_State.logicalDevice.acquireNextImageKHR(m_State.swapchain, 1'000'000'000, frameData.imageAvailableSemaphore);
		m_CurrentColorBufferIndex = image.value;*/
	}

	CommandBufferVulkan GraphicsDeviceVulkan::create_command_buffer_impl()
	{
		return CommandBufferVulkan(this);
	}

	texture_handle GraphicsDeviceVulkan::create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset)
	{
		Texture texture;
		texture.format = get_texture_format(descriptor.format);
		texture.extent = vk::Extent3D{ descriptor.width, descriptor.height, 1 };

		vk::ImageCreateInfo imageCreateInfo = {};
		{
			imageCreateInfo.flags = vk::ImageCreateFlags();
			imageCreateInfo.imageType = get_texture_type(descriptor.type);

			imageCreateInfo.format = texture.format;
			imageCreateInfo.extent = texture.extent;

			imageCreateInfo.mipLevels = descriptor.mipLevels;
			imageCreateInfo.arrayLayers = 1;

			// TODO: support multisampling.
			imageCreateInfo.samples = vk::SampleCountFlagBits::e1;

			// TODO: support linear tiling for CPU data.
			imageCreateInfo.tiling = vk::ImageTiling::eOptimal;

			// TODO: descriptor for usage flags.
			vk::ImageUsageFlags usageFlags;
			{
				usageFlags |= vk::ImageUsageFlagBits::eTransferSrc;
				usageFlags |= vk::ImageUsageFlagBits::eTransferDst;
				usageFlags |= vk::ImageUsageFlagBits::eStorage;
				usageFlags |= vk::ImageUsageFlagBits::eColorAttachment;
			}
			imageCreateInfo.usage = usageFlags;
		}

		VmaAllocationCreateInfo imageAllocationInfo = {};
		{
			imageAllocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
			imageAllocationInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		}
		
		if (vmaCreateImage(
			m_State.vmaAllocator,
			reinterpret_cast<const VkImageCreateInfo*>(&imageCreateInfo),
			&imageAllocationInfo,
			reinterpret_cast<VkImage*>(&texture.image),
			&texture.allocation,
			nullptr
		) != VK_SUCCESS)
		{
			TUR_LOG_ERROR("Failed to create image");
		}
		
		vk::ImageViewCreateInfo imageViewCreateInfo = {};
		{
			imageViewCreateInfo.flags = vk::ImageViewCreateFlags();
			imageViewCreateInfo.viewType = get_texture_view_type(descriptor.type);
			imageViewCreateInfo.image = texture.image;
			imageViewCreateInfo.format = texture.format;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			// TODO: descriptor aspect flags
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		}
		
		texture.imageView = m_State.logicalDevice.createImageView(imageViewCreateInfo);

		return m_Textures.add(texture);
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

	pipeline_handle GraphicsDeviceVulkan::create_pipeline_impl(const PipelineDescriptor& descriptor)
	{
		vk::Pipeline pipeline = create_graphics_pipeline(*this, descriptor);

		// TODO: destroy others
		destroy_shader(descriptor.vertexShader);
		destroy_shader(descriptor.fragmentShader);

		return m_Pipelines.add(pipeline);
	}
}