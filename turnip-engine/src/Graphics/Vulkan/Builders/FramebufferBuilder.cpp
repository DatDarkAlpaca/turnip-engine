#include "pch.h"
#include "Common.h"
#include "FramebufferBuilder.h"

namespace tur::vulkan 
{
	FramebufferBuilder& FramebufferBuilder::SetArguments(vk::Device device, vk::RenderPass renderpass)
	{
		m_Device = device;
		m_Renderpass = renderpass;

		m_ArgumentsSet = true;
		return *this;
	}

	vk::Framebuffer FramebufferBuilder::Create(const vk::ImageView& view, const Swapchain& swapchain)
	{
		if (!m_ArgumentsSet)
			TUR_LOG_CRITICAL("You must set the arguments before creating a framebuffer. Use SetArguments()");

		std::vector<vk::ImageView> attachments = {
			view
		};

		vk::FramebufferCreateInfo framebufferInfo = { };
		framebufferInfo.flags = vk::FramebufferCreateFlags();
		framebufferInfo.renderPass = m_Renderpass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchain.extent.width;
		framebufferInfo.height = swapchain.extent.height;
		framebufferInfo.layers = 1;

		try
		{
			return m_Device.createFramebuffer(framebufferInfo);
		}
		catch (vk::SystemError err)
		{
			TUR_LOG_ERROR("Failed to create framebuffer");
			return nullptr;
		}
	}
}