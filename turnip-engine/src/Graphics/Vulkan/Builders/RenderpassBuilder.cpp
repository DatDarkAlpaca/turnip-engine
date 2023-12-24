#include "pch.h"
#include "RenderpassBuilder.h"

namespace tur::vulkan
{
	RenderpassBuilder::RenderpassBuilder(const RenderpassDescriptor& descriptor)
		: m_Descriptor(descriptor)
	{

	}

	std::optional<RenderpassVulkan> RenderpassBuilder::Build()
	{
		if (!m_ArgumentsSet)
		{
			TUR_LOG_ERROR("[Vulkan Renderpass]: You must provide a valid device and swapchain to create a renderpass. Use SetArguments().");
			return std::nullopt;
		}

		if (m_Descriptor.defaultSwapchainAttachment)
			AddSwapchainColorAttachment();

		PrepareDescriptor();

		// Renderpass
		vk::RenderPassCreateInfo renderpassInfo = { };
		renderpassInfo.flags = vk::RenderPassCreateFlags();
		renderpassInfo.attachmentCount = m_Attachments.size();
		renderpassInfo.pAttachments = m_Attachments.data();
		renderpassInfo.subpassCount = m_Subpasses.size();
		renderpassInfo.pSubpasses = m_Subpasses.data();

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

	RenderpassBuilder& RenderpassBuilder::SetArguments(vk::Device device, const vulkan::Swapchain& swapchain)
	{
		m_Device = device;
		m_Swapchain = swapchain;
		m_ArgumentsSet = true;

		return *this;
	}

	void tur::vulkan::RenderpassBuilder::AddSwapchainColorAttachment()
	{
		vk::AttachmentDescription swapchainAttachment = { };
		swapchainAttachment.flags = vk::AttachmentDescriptionFlags();
		swapchainAttachment.format = m_Swapchain.surfaceFormat.format;
		swapchainAttachment.samples = vk::SampleCountFlagBits::e1;
		swapchainAttachment.loadOp = vk::AttachmentLoadOp::eClear;
		swapchainAttachment.storeOp = vk::AttachmentStoreOp::eStore;
		swapchainAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		swapchainAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		swapchainAttachment.initialLayout = vk::ImageLayout::eUndefined;
		swapchainAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

		m_Attachments.push_back(swapchainAttachment);
	}
	
	void RenderpassBuilder::PrepareDescriptor()
	{
		// Descriptors and references:
		for (const auto& descriptorAttachment : m_Descriptor.attachments)
		{
			vk::AttachmentDescription attachment = { };
			attachment.flags = vk::AttachmentDescriptionFlags();
			attachment.format = GetFormat(descriptorAttachment.imageFormat);
			attachment.samples = GetSampleCount(descriptorAttachment.sampleCount);
			attachment.loadOp = GetAttachmentLoadOp(descriptorAttachment.loadOperation);
			attachment.storeOp = GetAttachmentStoreOp(descriptorAttachment.storeOperation);
			attachment.stencilLoadOp = GetAttachmentLoadOp(descriptorAttachment.stencilLoadOperation);
			attachment.stencilStoreOp = GetAttachmentStoreOp(descriptorAttachment.stencilStoreOperation);
			attachment.initialLayout = GetImageLayout(descriptorAttachment.initialLayout);
			attachment.finalLayout = GetImageLayout(descriptorAttachment.finalLayout);

			m_Attachments.push_back(attachment);

			vk::AttachmentReference attachmentRef = { };
			attachmentRef.attachment = m_Attachments.size() - 1;
			attachmentRef.layout = GetImageLayout(descriptorAttachment.attachmentLayout);

			m_AttachmentReferences.push_back(attachmentRef);
		}

		// Subpasses:
		for (size_t i = 0; i < m_Descriptor.subpasses.size(); ++i)
		{
			vk::SubpassDescription subpass = { };
			subpass.flags = vk::SubpassDescriptionFlags();
			subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

			std::vector<vk::AttachmentReference> inputAttachments;
			std::vector<vk::AttachmentReference> colorAttachments;
			std::vector<vk::AttachmentReference> resolveAttachments;
			std::vector<vk::AttachmentReference> depthStencilAttachments;

			for (const auto& [subpassReferenceType, subpassReferenceIndex] : m_Descriptor.subpasses[i])
			{
				switch (subpassReferenceType)
				{
					case AttachmentType::INPUT:
						inputAttachments.push_back(m_AttachmentReferences[subpassReferenceIndex]);

					case AttachmentType::COLOR:
						colorAttachments.push_back(m_AttachmentReferences[subpassReferenceIndex]);

					case AttachmentType::RESOLVE:
						resolveAttachments.push_back(m_AttachmentReferences[subpassReferenceIndex]);

					case AttachmentType::DEPTH_STENCIL:
						depthStencilAttachments.push_back(m_AttachmentReferences[subpassReferenceIndex]);

					default:
					{
						TUR_LOG_ERROR("Invalid subpass attachment reference type at index: {}", subpassReferenceIndex);
					} break;
				}
				
			}

			subpass.inputAttachmentCount = inputAttachments.size();
			subpass.pInputAttachments = inputAttachments.data();

			subpass.colorAttachmentCount = colorAttachments.size();
			subpass.pColorAttachments = colorAttachments.data();

			subpass.pResolveAttachments = resolveAttachments.data();
			subpass.pDepthStencilAttachment = depthStencilAttachments.data();

			// TODO: Implement preserve attachments.

			m_Subpasses.push_back(subpass);
		}
	}
}