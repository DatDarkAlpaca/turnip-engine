#pragma once
#include <vulkan/vulkan.hpp>

#include "Common.h"
#include "ShaderVulkan.h"
#include "Graphics/Pipeline.h"
#include "Initialization/Swapchain.h"

namespace tur
{
	// Graphics Pipeline
	class PipelineVulkan : public Pipeline
	{
	public:
		vk::Pipeline pipeline;
		vk::PipelineLayout layout;
		vk::RenderPass renderpass;
	};

	class PipelineBuilder
	{
	public:
		explicit PipelineBuilder(const PipelineDescriptor& descriptor)
			: descriptor(descriptor) { }

	public:
		PipelineVulkan Build();

	private:
		vk::PipelineLayout CreateLayout() const
		{
			vk::PipelineLayoutCreateInfo layoutInfo = { };
			layoutInfo.flags = vk::PipelineLayoutCreateFlags();
			layoutInfo.setLayoutCount = descriptor.layoutCount;
			layoutInfo.pushConstantRangeCount = descriptor.pushConstantCount;

			try
			{
				return m_Device.createPipelineLayout(layoutInfo);
			}
			catch (vk::SystemError err)
			{
				TUR_LOG_ERROR("Failed to create pipeline layout: {}", err.what());
			}

			return nullptr;
		}

		vk::RenderPass CreateRenderpass() const
		{
			// TODO: Stop being lazy. Flexibility. 

			// Color Attachment:
			vk::AttachmentDescription colorAttachment = {};
			colorAttachment.flags = vk::AttachmentDescriptionFlags();
			colorAttachment.format = m_SwapchainFormat;
			colorAttachment.samples = vk::SampleCountFlagBits::e1;
			colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
			colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
			colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
			colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
			colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
			colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

			vk::AttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

			// Main Subpass:
			vk::SubpassDescription subpass = {};
			subpass.flags = vk::SubpassDescriptionFlags();
			subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			// Renderpass:
			vk::RenderPassCreateInfo renderpassInfo = {};
			renderpassInfo.flags = vk::RenderPassCreateFlags();
			renderpassInfo.attachmentCount = 1;
			renderpassInfo.pAttachments = &colorAttachment;
			renderpassInfo.subpassCount = 1;
			renderpassInfo.pSubpasses = &subpass;

			try {
				return m_Device.createRenderPass(renderpassInfo);
			}
			catch (vk::SystemError err)
			{
				TUR_LOG_ERROR("Failed to create renderpass. {}", err.what());
			}

			return nullptr;
		}

	public:
		PipelineBuilder& SetDevice(const vk::Device& device)
		{
			m_Device = device;
			m_DeviceSet = true;
			return *this;
		}

		PipelineBuilder& SetSwapchain(const vulkan::SwapchainData& swapchain)
		{
			m_SwapchainExtent = swapchain.extent;
			m_SwapchainFormat = swapchain.surfaceFormat.format;
			m_SwapchainSet = true;
			return *this;
		}

	private:
		// Device Input:
		vk::Device m_Device;
		bool m_DeviceSet = false;

		// Swapchain Input:
		vk::Extent2D m_SwapchainExtent;
		vk::Format m_SwapchainFormat;
		bool m_SwapchainSet = false;

		PipelineDescriptor descriptor;
	};
}