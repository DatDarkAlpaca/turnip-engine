#include "pch.h"
#include "RenderpassBuilder.h"

namespace tur::vulkan
{
	std::optional<RenderpassVulkan> RenderpassBuilder::Build()
	{
		if (!m_DeviceSet)
		{
			TUR_LOG_ERROR("[Vulkan Renderpass]: You must provide a valid device to create a renderpass. Use SetDevice().");
			return std::nullopt;
		}

		// Subpasses:
		vk::SubpassDescription subpass = { };
		subpass.flags = vk::SubpassDescriptionFlags();
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

		// TODO: implement AddReferences() and AddSubpass()
		vk::AttachmentReference colorAttachmentRef = { };
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		// Rendepass
		vk::RenderPassCreateInfo renderpassInfo = { };
		renderpassInfo.flags = vk::RenderPassCreateFlags();
		renderpassInfo.attachmentCount = m_Attachments.size();
		renderpassInfo.pAttachments = m_Attachments.data();
		renderpassInfo.subpassCount = 1;
		renderpassInfo.pSubpasses = &subpass;

		RenderpassVulkan renderpass;

		try
		{
			renderpass.renderpass = m_Device.createRenderPass(renderpassInfo);
			return renderpass;
		}
		catch (vk::SystemError err)
		{
			TUR_LOG_ERROR("Failed to create renderpass. {}", err.what());
		}

		return std::nullopt;
	}

	RenderpassBuilder& RenderpassBuilder::SetDevice(vk::Device device)
	{
		m_Device = device;
		m_DeviceSet = true;
		return *this;
	}

	uint32_t RenderpassBuilder::AddAttachment(uint32_t sampleCount, const AttachmentOperations& operations, ImageLayout initialLayout, ImageLayout finalLayout)
	{
		vk::AttachmentDescription attachment = { };
		attachment.flags = vk::AttachmentDescriptionFlags();
		// attachment.format = m_Swapchain.surfaceFormat.format;
		attachment.samples = GetSampleCount(sampleCount);;
		attachment.loadOp = GetAttachmentLoadOp(operations.attachmentLoad);
		attachment.storeOp = GetAttachmentStoreOp(operations.attachmentStore);
		attachment.stencilLoadOp = GetAttachmentLoadOp(operations.stencilLoad);
		attachment.stencilStoreOp = GetAttachmentStoreOp(operations.stencilStore);
		attachment.initialLayout = GetImageLayout(initialLayout);
		attachment.finalLayout = GetImageLayout(finalLayout);

		m_Attachments.push_back(attachment);
		return m_Attachments.size() - 1;
	}

	uint32_t RenderpassBuilder::AddAttachment(uint32_t sampleCount, const Swapchain& swapchain, const AttachmentOperations& operations, ImageLayout initialLayout, ImageLayout finalLayout)
	{
		vk::AttachmentDescription attachment = { };
		attachment.flags = vk::AttachmentDescriptionFlags();
		attachment.format = swapchain.surfaceFormat.format;
		attachment.samples = GetSampleCount(sampleCount);;
		attachment.loadOp = GetAttachmentLoadOp(operations.attachmentLoad);
		attachment.storeOp = GetAttachmentStoreOp(operations.attachmentStore);
		attachment.stencilLoadOp = GetAttachmentLoadOp(operations.stencilLoad);
		attachment.stencilStoreOp = GetAttachmentStoreOp(operations.stencilStore);
		attachment.initialLayout = GetImageLayout(initialLayout);
		attachment.finalLayout = GetImageLayout(finalLayout);

		m_Attachments.push_back(attachment);
		return m_Attachments.size() - 1;
	}

	uint32_t tur::vulkan::RenderpassBuilder::AddDefaultColorAttachment(const Swapchain& swapchain)
	{
		vk::AttachmentDescription colorAttachment = { };
		colorAttachment.flags = vk::AttachmentDescriptionFlags();
		colorAttachment.format = swapchain.surfaceFormat.format;
		colorAttachment.samples = vk::SampleCountFlagBits::e1;
		colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
		colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
		colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;;
		colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
		colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

		m_Attachments.push_back(colorAttachment);
		return m_Attachments.size() - 1;
	}

	RenderpassBuilder& RenderpassBuilder::AddReference()
	{
		TUR_LOG_WARN("NOT IMPLEMENTED: RenderpassBuilder::AddReference()");
		return *this;
	}
}
