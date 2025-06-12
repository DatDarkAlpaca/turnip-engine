#include "pch.hpp"
#include "vulkan_device.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_gui_system.hpp"
#include "platform/vulkan_context.hpp"

#include "builders/instance_builder.hpp"
#include "builders/physical_device_builder.hpp"
#include "builders/logical_device_builder.hpp"
#include "builders/vma_allocator_builder.hpp"
#include "builders/command_pool_builder.hpp"
#include "builders/swapchain_builder.hpp"
#include "builders/frame_data_builder.hpp"

#include "factories/shader_factory.hpp"
#include "factories/pipeline_factory.hpp"
#include "factories/buffer_factory.hpp"
#include "factories/texture_factory.hpp"
#include "factories/descriptor_factory.hpp"

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

		// Pools:
		initialize_command_pool(m_State);

		// Frame Data:
		initialize_frame_data(m_State);

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

		// Texture:
		TextureDescriptor descriptor;
		{
			descriptor.format = get_texture_format(m_State.swapchainFormat.format);
			descriptor.width = m_State.swapchainExtent.width;
			descriptor.height = m_State.swapchainExtent.height;

			m_State.drawTextureHandle = create_texture(descriptor, {});
			m_State.drawTexture = m_Textures.get(m_State.drawTextureHandle);
		}

		// Deletion queue:
		deletion::initialize_deletion_queue(m_DeletionQueue, this);
	}
	void GraphicsDeviceVulkan::shutdown_impl()
	{
		auto& device = m_State.logicalDevice;
		wait_idle();

		deletion::flush(m_DeletionQueue);

		// Swapchain:
		{
			for (const auto& image : m_State.swapChainImageViews)
				device.destroyImageView(image);

			device.destroySwapchainKHR(m_State.swapchain);
		}

		// Imm Command buffer:
		{
			device.destroyCommandPool(m_ImmCommandPool);
			device.destroyFence(m_ImmFence);
		}

		// Resources:
		{
			for (const auto& pipeline : m_Pipelines.available())
			{
				device.destroyPipelineLayout(pipeline.layout);
				device.destroyPipeline(pipeline.pipeline);
			}

			for (const auto& descriptor : m_Descriptors.available())
			{
				device.destroyDescriptorPool(descriptor.descriptorPool);
				device.destroyDescriptorSetLayout(descriptor.setLayout);
			}

			for (const auto& texture : m_Textures.available())
			{
				vmaDestroyImage(m_State.vmaAllocator, texture.image, texture.allocation);
				device.destroyImageView(texture.imageView);
				device.destroySampler(texture.sampler);
			}

			for (auto& buffer : m_Buffers.available())
				vmaDestroyBuffer(m_State.vmaAllocator, buffer.buffer, buffer.allocation);

			m_ShaderModules.clear();
			m_RenderTargets.clear();
			m_Descriptors.clear();
			m_Pipelines.clear();
			m_Textures.clear();
			m_Buffers.clear();

			m_State.frameDataHolder.shutdown(device);
			device.destroyCommandPool(m_State.commandPool);
		}
		
		vmaDestroyAllocator(m_State.vmaAllocator);
		device.destroy();

		destroy_instance_messenger(m_State);

		vkDestroySurfaceKHR(m_State.instance, m_State.surface, nullptr);
		m_State.instance.destroy();
	}

	void GraphicsDeviceVulkan::begin_impl()
	{
		auto& device = m_State.logicalDevice;
		auto& swapchain = m_State.swapchain;
		auto& frameDataHolder = m_State.frameDataHolder;
		auto& frameData = frameDataHolder.get_frame_data();

		try
		{
			auto result = device.waitForFences(frameData.recordingFence, true, 1'000'000'000);
			device.resetFences(frameData.recordingFence);
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_CRITICAL("Failed to wait for fences. {}", err.what());
		}

		deletion::flush(m_DeletionQueue);

		auto imageResult = device.acquireNextImageKHR(swapchain, 1'000'000'000, frameData.imageAvailableSemaphore);
		frameDataHolder.set_color_buffer(imageResult.value);

		if (imageResult.result == vk::Result::eErrorOutOfDateKHR)
			recreate_swapchain();

		else if (imageResult.result != vk::Result::eSuccess && imageResult.result != vk::Result::eSuboptimalKHR)
			TUR_LOG_CRITICAL("Failed to acquire swapchain image");

		auto& currentCommandBuffer = frameData.commandBuffer;
		currentCommandBuffer.reset();

		vk::CommandBufferBeginInfo beginInfo = {};
		{
			beginInfo.pInheritanceInfo = nullptr;
			beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		}

		try
		{
			currentCommandBuffer.begin(beginInfo);
		}
		catch (vk::SystemError& err) {
			TUR_LOG_ERROR("Failed to begin() recording to vulkan command buffer.", err.what());
		}
	}
	void GraphicsDeviceVulkan::submit_impl()
	{
		auto& graphicsQueue = m_State.queueList.get(QueueUsage::GRAPHICS);
		auto& frameDataHolder = m_State.frameDataHolder;
		const auto& frameData = frameDataHolder.get_frame_data();

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

	void GraphicsDeviceVulkan::end_impl()
	{
		auto& currentCommandBuffer = m_State.frameDataHolder.get_frame_data().commandBuffer;

		try
		{
			currentCommandBuffer.end();
		}
		catch (vk::SystemError err)
		{
			throw std::runtime_error("Failed to end() recording to vulkan command buffer.");
		}
	}

	CommandBufferVulkan GraphicsDeviceVulkan::create_command_buffer_impl()
	{
		return CommandBufferVulkan(this);
	}
	VulkanGUI GraphicsDeviceVulkan::create_gui_system_impl()
	{
		return VulkanGUI(this);
	}

	shader_handle GraphicsDeviceVulkan::create_shader_impl(const ShaderDescriptor& descriptor)
	{
		vk::ShaderModule shaderModule = build_shader_module(m_State.logicalDevice, descriptor);
		return m_ShaderModules.add(shaderModule);
	}
	void GraphicsDeviceVulkan::destroy_shader_impl(shader_handle textureHandle)
	{
		auto shaderModule = m_ShaderModules.get(textureHandle);
		m_State.logicalDevice.destroyShaderModule(shaderModule);
		m_ShaderModules.remove(textureHandle);
	}

	descriptor_handle GraphicsDeviceVulkan::create_descriptors_impl(const DescriptorSetLayoutDescriptor& descriptor)
	{
		Descriptor descriptorWrapper;
		descriptorWrapper.descriptorPool = vulkan::build_descriptor_pool(m_State.logicalDevice, descriptor);
		descriptorWrapper.setLayout = vulkan::build_descriptor_set_layout(m_State.logicalDevice, descriptor);
		
		return m_Descriptors.add(descriptorWrapper);
	}
	descriptor_set_handle GraphicsDeviceVulkan::create_descriptor_set_impl(descriptor_handle textureHandle)
	{
		Descriptor& descriptor = m_Descriptors.get(textureHandle);

		DescriptorSet set = {};
		set.descriptorHandle = textureHandle;
		set.set = vulkan::build_descriptor_set(m_State.logicalDevice, descriptor.descriptorPool, { descriptor.setLayout });

		return m_DescriptorSets.add(set);
	}
	void GraphicsDeviceVulkan::update_descriptor_resource_impl(descriptor_set_handle descriptorSet, handle_type resourceHandle, DescriptorType type, u32 binding)
	{
		switch (type)
		{
			case DescriptorType::UNIFORM_BUFFER:
			case DescriptorType::STORAGE_BUFFER:
			{
				const Buffer& buffer = get_buffers().get(resourceHandle);

				vk::DescriptorBufferInfo bufferInfo = {};
				{
					bufferInfo.buffer = buffer.buffer;
					bufferInfo.offset = 0;
					bufferInfo.range = buffer.size;
				}

				vk::WriteDescriptorSet descriptorWrite = {};
				{
					descriptorWrite.dstSet = m_DescriptorSets.get(descriptorSet).set;
					descriptorWrite.dstBinding = binding;
					descriptorWrite.dstArrayElement = 0;
					descriptorWrite.descriptorType = get_descriptor_type(type);
					descriptorWrite.descriptorCount = 1;

					descriptorWrite.pBufferInfo = &bufferInfo;
				}

				m_State.logicalDevice.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
			} break;

			case DescriptorType::COMBINED_IMAGE_SAMPLER:
			{
				const Texture& texture = get_textures().get(resourceHandle);

				vk::DescriptorImageInfo imageInfo = {};
				{
					imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
					imageInfo.imageView = texture.imageView;
					imageInfo.sampler = texture.sampler;
				}

				vk::WriteDescriptorSet descriptorWrite = {};
				{
					descriptorWrite.dstSet = m_DescriptorSets.get(descriptorSet).set;
					descriptorWrite.dstBinding = binding;
					descriptorWrite.dstArrayElement = 0;
					descriptorWrite.descriptorType = get_descriptor_type(type);
					descriptorWrite.descriptorCount = 1;

					descriptorWrite.pImageInfo = &imageInfo;
				}

				m_State.logicalDevice.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
			} break;
		}
	}

	pipeline_handle GraphicsDeviceVulkan::create_graphics_pipeline_impl(const PipelineDescriptor& descriptor)
	{
		Pipeline pipeline;

		// Pipeline Creation:
		pipeline = vulkan::build_graphics_pipeline(*this, descriptor);
		
		destroy_shader(descriptor.vertexShader);

		if (descriptor.tesselationControlShader != invalid_handle)
			destroy_shader(descriptor.tesselationControlShader);

		if (descriptor.tesselationEvaluationShader != invalid_handle)
			destroy_shader(descriptor.tesselationEvaluationShader);

		if (descriptor.geometryShader != invalid_handle)
			destroy_shader(descriptor.geometryShader);

		destroy_shader(descriptor.fragmentShader);

		return m_Pipelines.add(pipeline);
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
		Buffer buffer = vulkan::build_buffer(m_State.vmaAllocator, srcDescriptor, data.size);

		// Staging buffer:
		BufferDescriptor stagingDescriptor = {};
		{
			stagingDescriptor.memoryUsage = BufferMemoryUsage::CPU_ONLY;
			stagingDescriptor.type = BufferType::TRANSFER_SRC;
		}
		Buffer stagingBuffer = vulkan::build_buffer(m_State.vmaAllocator, stagingDescriptor, data.size);
		
		// Buffer data:
		if (data.data)
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
		return m_Buffers.add(vulkan::build_buffer(m_State.vmaAllocator, descriptor, size));
	}
	void GraphicsDeviceVulkan::update_buffer_impl(buffer_handle textureHandle, const DataBuffer& data, u32 offset)
	{
		Buffer& targetBuffer = m_Buffers.get(textureHandle);

		// Staging buffer:
		BufferDescriptor stagingDescriptor = {};
		{
			stagingDescriptor.memoryUsage = BufferMemoryUsage::CPU_ONLY;
			stagingDescriptor.type = BufferType::TRANSFER_SRC;
		}
		Buffer stagingBuffer = vulkan::build_buffer(m_State.vmaAllocator, stagingDescriptor, data.size);
		
		{
			void* bufferData = nullptr;
			vmaMapMemory(m_State.vmaAllocator, stagingBuffer.allocation, &bufferData);
			std::memcpy(bufferData, (u8*)data.data + offset, data.size);
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

			m_ImmCommandBuffer.copyBuffer(stagingBuffer.buffer, targetBuffer.buffer, region);
		});

		vmaDestroyBuffer(m_State.vmaAllocator, stagingBuffer.buffer, stagingBuffer.allocation);
	}
	void* GraphicsDeviceVulkan::map_buffer_impl(buffer_handle textureHandle, u32 offset, u32 length, AccessFlags flags)
	{
		TUR_LOG_CRITICAL("map_buffer not implemented");
		return nullptr;
	}
	void GraphicsDeviceVulkan::copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset, u32 dstOffset)
	{
		submit_immediate_command([&]() {
			vk::BufferCopy region;
			{
				region.dstOffset = dstOffset;
				region.srcOffset = srcOffset;
				region.size = size;
			}

			Buffer& srcBuffer = m_Buffers.get(source);
			Buffer& dstBuffer = m_Buffers.get(destination);

			m_ImmCommandBuffer.copyBuffer(srcBuffer.buffer, dstBuffer.buffer, region);
		});
	}
	void GraphicsDeviceVulkan::copy_buffer_to_texture_impl(buffer_handle source, texture_handle destination, u32 width, u32 height)
	{
		copy_buffer_to_texture_direct(m_Buffers.get(source), m_Textures.get(destination), width, height);
	}
	void GraphicsDeviceVulkan::destroy_buffer_impl(buffer_handle& textureHandle)
	{
		deletion::destroy_buffer(m_DeletionQueue, textureHandle);
	}

	texture_handle GraphicsDeviceVulkan::create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset)
	{
		return m_Textures.add(vulkan::build_texture(this, descriptor, asset));
	}
	texture_handle GraphicsDeviceVulkan::create_texture_impl(const TextureDescriptor& descriptor)
	{
		return create_texture(descriptor, {});
	}
	void GraphicsDeviceVulkan::update_texture_impl(texture_handle textureHandle, const TextureAsset& asset)
	{
		// TODO
	}
	void GraphicsDeviceVulkan::destroy_texture_impl(texture_handle& textureHandle)
	{
		deletion::destroy_texture(m_DeletionQueue, textureHandle);
	}

	render_target_handle GraphicsDeviceVulkan::create_render_target_impl(const RenderTargetDescriptor& descriptor)
	{
		bool hasColorAttachment = false;
		for (const auto& attachment : descriptor.colorAttachments)
		{
			if (attachment != invalid_handle)
				hasColorAttachment = true;
		}

		RenderTarget renderTarget;
		renderTarget.descriptor = descriptor;
		
		if (!hasColorAttachment)
		{
			TextureDescriptor textureDescriptor;
			{
				textureDescriptor.format = get_texture_format(m_State.swapchainFormat.format);
				textureDescriptor.width = descriptor.width;
				textureDescriptor.height = descriptor.height;
			}

			texture_handle colorAttachment = create_texture(textureDescriptor);
			renderTarget.descriptor.colorAttachments.push_back(colorAttachment);
		}

		return m_RenderTargets.add(renderTarget);
	}
	void GraphicsDeviceVulkan::resize_render_target_impl(render_target_handle& textureHandle, u32 width, u32 height)
	{
		destroy_render_target(textureHandle);

		RenderTargetDescriptor descriptor = {};
		{
			descriptor.width = width;
			descriptor.height = height;
		}

		textureHandle = create_render_target(descriptor);
	}
	void GraphicsDeviceVulkan::destroy_render_target_impl(render_target_handle& textureHandle)
	{
		deletion::destroy_render_target(m_DeletionQueue, textureHandle);
		return;
		RenderTarget& renderTarget = m_RenderTargets.get(textureHandle);

		for (const auto& colorAttachment : renderTarget.descriptor.colorAttachments)
		{
			Texture& texture = m_Textures.get(colorAttachment);
			m_State.logicalDevice.destroyImageView(texture.imageView);
			vmaDestroyImage(m_State.vmaAllocator, texture.image, texture.allocation);
			
			m_Textures.remove(colorAttachment);
		}

		if (renderTarget.descriptor.depthAttachment != invalid_handle)
		{
			Texture& texture = m_Textures.get(renderTarget.descriptor.depthAttachment);
			m_State.logicalDevice.destroyImageView(texture.imageView);
			vmaDestroyImage(m_State.vmaAllocator, texture.image, texture.allocation);
		}

		m_RenderTargets.remove(textureHandle);
		textureHandle = invalid_handle;
	}
}

namespace tur::vulkan
{
	void GraphicsDeviceVulkan::wait_idle()
	{
		m_State.logicalDevice.waitIdle();
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

		auto a = m_State.queueList.get(QueueUsage::TRANSFER);
		a.submit2(submitInfo, m_ImmFence);
		auto result = m_State.logicalDevice.waitForFences(m_ImmFence, true, 1000000000);
	}
	void GraphicsDeviceVulkan::transition_texture_layout(Texture& texture, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
	{
		submit_immediate_command([&]() {

			vk::ImageAspectFlags aspectMask;
			vk::ImageSubresourceRange subresourceRange;

			vk::ImageMemoryBarrier2 imageBarrier = {};
			{
				imageBarrier.srcStageMask = vk::PipelineStageFlagBits2::eAllCommands;
				imageBarrier.srcAccessMask = vk::AccessFlagBits2::eMemoryRead;

				imageBarrier.dstStageMask = vk::PipelineStageFlagBits2::eAllCommands;
				imageBarrier.dstAccessMask = vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead;

				imageBarrier.oldLayout = oldLayout;
				imageBarrier.newLayout = newLayout;

				if (newLayout == vk::ImageLayout::eDepthAttachmentOptimal)
					aspectMask = vk::ImageAspectFlagBits::eDepth;
				else
					aspectMask = vk::ImageAspectFlagBits::eColor;

				subresourceRange.aspectMask = aspectMask;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

				imageBarrier.subresourceRange = subresourceRange;
				imageBarrier.image = texture.image;
			}

			vk::DependencyInfo dependencyInfo = vk::DependencyInfo().setImageMemoryBarriers(imageBarrier);

			try
			{
				m_ImmCommandBuffer.pipelineBarrier2(dependencyInfo);
			}
			catch (vk::SystemError& err)
			{
				TUR_LOG_CRITICAL("Failed to issue transition image layout command. {}", err.what());
			}
			});
	}
	void GraphicsDeviceVulkan::transition_texture_layout(texture_handle textureHandle, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
	{
		transition_texture_layout(m_Textures.get(textureHandle), oldLayout, newLayout);
	}
	void GraphicsDeviceVulkan::copy_buffer_to_texture_direct(Buffer& buffer, Texture& texture, u32 width, u32 height)
	{
		submit_immediate_command([&]() {
			vk::ImageAspectFlags aspectMask = vk::ImageAspectFlagBits::eColor;

			vk::BufferImageCopy2 region;
			{
				region.bufferOffset = 0;
				region.bufferRowLength = 0;
				region.bufferImageHeight = 0;

				region.imageSubresource.aspectMask = aspectMask;
				region.imageSubresource.mipLevel = 0;
				region.imageSubresource.baseArrayLayer = 0;
				region.imageSubresource.layerCount = 1;
				region.imageOffset = vk::Offset3D(0, 0, 0);
				region.imageExtent = vk::Extent3D(width, height, 1);
			}

			vk::CopyBufferToImageInfo2 copyInfo = {};
			{
				copyInfo.srcBuffer = buffer.buffer;
				copyInfo.dstImage = texture.image;
				copyInfo.dstImageLayout = vk::ImageLayout::eTransferDstOptimal;
				copyInfo.regionCount = 1;
				copyInfo.pRegions = &region;
			}

			m_ImmCommandBuffer.copyBufferToImage2(copyInfo);
		});
	}
}

namespace tur::vulkan
{
	void GraphicsDeviceVulkan::recreate_swapchain()
	{
		auto size = get_window_size(r_Window);

		while (size.x == 0 || size.y == 0)
			size = get_window_size(r_Window);

		wait_idle();
		cleanup_swapchain(m_State);

		// New Swapchain:
		SwapchainRequirements requirements;
		requirements.extent = Extent2D{ size.x, size.y };

		initialize_swapchain(m_State, requirements);

		initialize_frame_data(m_State);

		// Recreate main texture:

		TextureDescriptor descriptor;
		{
			descriptor.format = get_texture_format(m_State.swapchainFormat.format);
			descriptor.width = m_State.swapchainExtent.width;
			descriptor.height = m_State.swapchainExtent.height;

			destroy_texture(m_State.drawTextureHandle);
			m_State.drawTextureHandle = create_texture(descriptor, {});
			m_State.drawTexture = m_Textures.get(m_State.drawTextureHandle);
		}
	}
}