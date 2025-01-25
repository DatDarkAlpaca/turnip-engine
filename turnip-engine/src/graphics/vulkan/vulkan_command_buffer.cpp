#include "pch.hpp"
#include "vulkan_command_buffer.hpp"

namespace tur::vulkan
{
	CommandBufferVulkan::CommandBufferVulkan(NON_OWNING GraphicsDeviceVulkan* device)
		: r_Device(device)
	{
	}

	void CommandBufferVulkan::initialize_impl()
	{
	}

	void CommandBufferVulkan::begin_impl()
	{
		auto& device = r_Device->get_state().logicalDevice;
		auto& swapchain = r_Device->get_state().swapchain;
		auto& frameDataHolder = r_Device->get_state().frameDataHolder;
		auto& frameData = frameDataHolder.get_frame_data();

		try {
			auto result = device.waitForFences(frameData.recordingFence, true, 1'000'000'000);
			device.resetFences(frameData.recordingFence);
		}
		catch (vk::SystemError& err) {
			TUR_LOG_CRITICAL("Failed to wait for fences. {}", err.what());
		}

		u32 imageIndex = device.acquireNextImageKHR(swapchain, 1'000'000'000, frameData.imageAvailableSemaphore).value;
		frameDataHolder.set_color_buffer(imageIndex);

		m_CommandBuffer = get_command_buffer();
		m_CommandBuffer.reset();

		vk::CommandBufferBeginInfo beginInfo = {};
		{
			beginInfo.pInheritanceInfo = nullptr;
			beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		}

		try
		{
			m_CommandBuffer.begin(beginInfo);
		}
		catch (vk::SystemError& err) {
			TUR_LOG_ERROR("Failed to begin() recording to vulkan command buffer.", err.what());
		}

		auto& images = r_Device->get_state().swapChainImages;
		const auto& currentImage = frameDataHolder.get_color_buffer();

		transition_image(images.at(currentImage), vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral);
	}

	void CommandBufferVulkan::begin_render_impl()
	{
		const auto& currentFrame = r_Device->get_state().frameDataHolder.get_color_buffer();
		auto& imageView = r_Device->get_state().swapChainImageViews[currentFrame];
		const auto& swapchainExtent = r_Device->get_state().swapchainExtent;

		vk::RenderingInfo renderInfo = {};
		{
			VkRenderingAttachmentInfoKHR;
			vk::RenderingAttachmentInfo colorAttachmentInfo = {};
			{
				colorAttachmentInfo.imageView = imageView;
				colorAttachmentInfo.imageLayout = vk::ImageLayout::eAttachmentOptimal;
				colorAttachmentInfo.resolveMode = vk::ResolveModeFlagBits::eNone;
				colorAttachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
				colorAttachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;
				colorAttachmentInfo.clearValue = vk::ClearValue({ 0.0f, 1.0f, 0.0f, 0.0f });
			}

			/*vk::RenderingAttachmentInfo depthAttachmentInfo = {};
			{
				colorAttachmentInfo.imageView;
				colorAttachmentInfo.imageLayout;
				colorAttachmentInfo.resolveMode;
				colorAttachmentInfo.resolveImageView;
				colorAttachmentInfo.resolveImageLayout;
				colorAttachmentInfo.loadOp;
				colorAttachmentInfo.storeOp;
				colorAttachmentInfo.clearValue;
			}*/

			renderInfo.renderArea = vk::Rect2D({}, { swapchainExtent.width, swapchainExtent.height });
			renderInfo.colorAttachmentCount = 1;
			renderInfo.pColorAttachments = &colorAttachmentInfo;
			renderInfo.layerCount = 1;
			// render_info.pDepthAttachment = &depthAttachmentInfo;
			// render_info.pStencilAttachment = &depthAttachmentInfo;
		}
		
		m_CommandBuffer.beginRendering(renderInfo);
	}
	void CommandBufferVulkan::end_render_impl()
	{
		m_CommandBuffer.endRendering();
	}
	void CommandBufferVulkan::end_impl()
	{
		auto& frameDataHolder = r_Device->get_state().frameDataHolder;

		auto& images = r_Device->get_state().swapChainImages;
		const auto& currentImage = frameDataHolder.get_color_buffer();

		transition_image(images.at(currentImage), vk::ImageLayout::eGeneral, vk::ImageLayout::ePresentSrcKHR);

		try {
			m_CommandBuffer.end();
		}
		catch (vk::SystemError err) {
			throw std::runtime_error("Failed to end() recording to vulkan command buffer.");
		}
	}
}