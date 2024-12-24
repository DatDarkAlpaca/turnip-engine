#include "pch.hpp"
#include "RenderpassBuilder.hpp"
#include "Graphics/Vulkan/TypeConverters.hpp"
#include "Graphics/Vulkan/VulkanInitializer.hpp"

// TODO: redo the entire renderpass system for vulkan

namespace tur::vulkan
{
	RenderpassBuilder::RenderpassBuilder(const RenderpassDescriptor& descriptor)
		: m_Descriptor(descriptor)
	{

	}

	std::optional<RenderpassObject> RenderpassBuilder::Build()
	{
		if (!m_ArgumentsSet)
		{
			TUR_LOG_ERROR("[Vulkan Renderpass]: You must provide a valid device and swapchain to create a renderpass. Use SetArguments().");
			return std::nullopt;
		}

		return ConfigureRenderpass();
	}

	RenderpassBuilder& RenderpassBuilder::SetArguments(vk::Device device, const SwapchainObject& swapchain)
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

		vk::AttachmentReference swapchainAttachmentRef = { };
		swapchainAttachmentRef.attachment = 0;
		swapchainAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

		vk::SubpassDescription mainSubpass = { };
		mainSubpass.flags = vk::SubpassDescriptionFlags();
		mainSubpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

		m_Subpasses.push_back(mainSubpass);
	}

	std::optional<RenderpassObject> RenderpassBuilder::ConfigureRenderpass()
	{
		// Descriptors and references:
		for (const auto& descriptorAttachment : m_Descriptor.attachments)
		{
			vk::AttachmentDescription attachment = { };
			attachment.flags = vk::AttachmentDescriptionFlags();
			attachment.format = GetTextureFormat(descriptorAttachment.imageFormat);
			attachment.samples = GetSampleCount(descriptorAttachment.sampleCount);
			attachment.loadOp = GetAttachmentLoadOp(descriptorAttachment.loadOperation);
			attachment.storeOp = GetAttachmentStoreOp(descriptorAttachment.storeOperation);
			attachment.stencilLoadOp = GetAttachmentLoadOp(descriptorAttachment.stencilLoadOperation);
			attachment.stencilStoreOp = GetAttachmentStoreOp(descriptorAttachment.stencilStoreOperation);
			attachment.initialLayout = GetImageLayout(descriptorAttachment.initialLayout);
			attachment.finalLayout = GetImageLayout(descriptorAttachment.finalLayout);

			m_Attachments.push_back(attachment);

			vk::AttachmentReference attachmentRef = { };
			attachmentRef.attachment = (uint32_t)m_Attachments.size() - 1;
			attachmentRef.layout = GetImageLayout(descriptorAttachment.attachmentLayout);

			m_AttachmentReferences.push_back(attachmentRef);
		}

		// TODO: brother of christ
		std::vector<std::vector<vk::AttachmentReference>> inputAttachments(8);
		std::vector<std::vector<vk::AttachmentReference>> colorAttachments(8);
		std::vector<std::vector<vk::AttachmentReference>> resolveAttachments(8);
		std::vector<std::vector<vk::AttachmentReference>> depthStencilAttachments(8);

		// Subpasses:
		for (size_t i = 0; i < m_Descriptor.subpasses.size(); ++i)
		{
			vk::SubpassDescription subpass = { };
			subpass.flags = vk::SubpassDescriptionFlags();
			subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

			// TODO: convert resolve and depth refs into stack arrays

			for (const auto& attachmentType : m_Descriptor.mapping[i])
			{
				auto passID = i;

				switch (attachmentType)
				{
					case AttachmentType::INPUT:
						inputAttachments[i].push_back(m_AttachmentReferences[passID]);
						break;

					case AttachmentType::COLOR:
						colorAttachments[i].push_back(m_AttachmentReferences[passID]);
						break;

					case AttachmentType::RESOLVE:
						resolveAttachments[i].push_back(m_AttachmentReferences[passID]);
						break;

					case AttachmentType::DEPTH_STENCIL:
						depthStencilAttachments[i].push_back(m_AttachmentReferences[passID]);
						break;

					default:
					{
						TUR_LOG_ERROR("Invalid subpass attachment reference type at index: {}", passID);
					} break;
				}
				
			}

			subpass.inputAttachmentCount = (uint32_t)inputAttachments[i].size();
			subpass.pInputAttachments = inputAttachments[i].data();

			subpass.colorAttachmentCount = (uint32_t)colorAttachments[i].size();
			subpass.pColorAttachments = colorAttachments[i].data();

			subpass.pResolveAttachments = resolveAttachments[i].data();
			subpass.pDepthStencilAttachment = depthStencilAttachments[i].data();

			// TODO: Implement preserve attachments.

			m_Subpasses.push_back(subpass);
		}		
	
		// Renderpass Creation:
		vk::RenderPassCreateInfo renderpassInfo = { };
		renderpassInfo.flags = vk::RenderPassCreateFlags();
		renderpassInfo.attachmentCount = (uint32_t)m_Attachments.size();
		renderpassInfo.pAttachments = m_Attachments.data();
		renderpassInfo.subpassCount = (uint32_t)m_Subpasses.size();
		renderpassInfo.pSubpasses = m_Subpasses.data();

		RenderpassObject renderpass;
		renderpass.offset = vk::Offset2D(m_Descriptor.offset.x, m_Descriptor.offset.y);
		renderpass.extent = vk::Extent2D(m_Descriptor.extent.width, m_Descriptor.extent.height);
		
		try
		{
			renderpass.renderpass = m_Device.createRenderPass(renderpassInfo);
			return renderpass;
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create renderpass. {}", err.what());
		}

		return std::nullopt;
	}
}