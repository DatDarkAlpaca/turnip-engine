#pragma once
#include <vulkan/vulkan.hpp>

#include "SwapchainFrame.h"
#include "Initialization/Swapchain.h"

namespace tur
{
	class FramebufferBuilder
	{
	public:
		FramebufferBuilder() = default;


	public:
		FramebufferBuilder& SetArguments(vk::Device device, vk::RenderPass renderpass, vulkan::SwapchainData& swapchain)
		{
			m_Device = device;
			m_Renderpass = renderpass;
			m_Swapchain = swapchain;

			m_ArgumentsSet = true;
		}

	public:
		void Create()
		{
			if(!m_ArgumentsSet)
				TUR_LOG_CRITICAL("You must set the arguments before creating a framebuffer. Use SetArguments()");

			for (auto& frame : m_Swapchain.frames)
			{
				std::vector<vk::ImageView> attachments = {
					frame.view
				};

				vk::FramebufferCreateInfo framebufferInfo = { };
				framebufferInfo.flags = vk::FramebufferCreateFlags();
				framebufferInfo.renderPass = m_Renderpass;
				framebufferInfo.attachmentCount = attachments.size();
				framebufferInfo.pAttachments = attachments.data();
				framebufferInfo.width = m_Swapchain.extent.width;
				framebufferInfo.height = m_Swapchain.extent.height;
				framebufferInfo.layers = 1;

				try
				{
					frame.framebuffer = m_Device.createFramebuffer(framebufferInfo);
				}
				catch (vk::SystemError err) 
				{
					TUR_LOG_ERROR("Failed to create framebuffer");
					return;
				}
			}
		}

	private:
		vk::Device m_Device;
		vk::RenderPass m_Renderpass;
		vulkan::SwapchainData m_Swapchain;
		bool m_ArgumentsSet = false;
	};

	inline void CreateFramebuffer()
	{

	}
}