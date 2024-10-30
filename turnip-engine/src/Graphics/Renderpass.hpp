#pragma once
#include <optional>
#include <unordered_map>
#include "CommonGraphics.hpp"

#include "Graphics/Type/Types.hpp"
#include "Framebuffer.hpp"

namespace tur
{
	using renderpass_handle = handle_type;

	struct RenderpassAttachment
	{
		uint32_t sampleCount = 1;
		AttachmentLoadOperation loadOperation = AttachmentLoadOperation::CLEAR;
		AttachmentStoreOperation storeOperation = AttachmentStoreOperation::STORE;
		AttachmentLoadOperation stencilLoadOperation = AttachmentLoadOperation::DONT_CARE;
		AttachmentStoreOperation stencilStoreOperation = AttachmentStoreOperation::DONT_CARE;

		TextureFormat imageFormat = TextureFormat::B8G8R8A8_UNORM;
		ImageLayout initialLayout = ImageLayout::UNDEFINED;
		ImageLayout finalLayout = ImageLayout::PRESENT_SRC;
		ImageLayout attachmentLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL;
	};

	struct RenderpassDescriptor
	{
		using subpass_id = uint32_t;
		using attachment_id = uint32_t;

	public:
		static RenderpassDescriptor CreateDefaultRenderpass(const Extent2D& extent)
		{
			RenderpassDescriptor descriptor;
			descriptor.pipelineType = PipelineType::GRAPHICS;
			descriptor.extent = extent;

			auto attachmentID = descriptor.AddAttachment(RenderpassAttachment{});
			auto subpassID = descriptor.AddSubpass();
			descriptor.AddSubpassMapping(attachmentID, subpassID, AttachmentType::COLOR);

			return descriptor;
		}

	public:
		attachment_id AddAttachment(const RenderpassAttachment& attachment)
		{
			attachments.push_back(attachment);
			return (uint32_t)attachments.size() - 1;
		}

		subpass_id AddSubpass()
		{
			subpasses.push_back(subpasses.size());
			return (uint32_t)subpasses.size() - 1;
		}

		void AddSubpassMapping(attachment_id attachmentIndex, subpass_id subpassIndex, AttachmentType type = AttachmentType::COLOR)
		{
			TUR_ASSERT(subpassIndex < subpasses.size(), "Invalid subpass index");
			mapping[attachmentIndex].push_back(type);
		}

	public:
		PipelineType pipelineType = PipelineType::GRAPHICS;
		framebuffer_handle framebuffer_handle;

	public:
		std::unordered_map<attachment_id, std::vector<AttachmentType>> mapping;
		std::vector<RenderpassAttachment> attachments;
		std::vector<subpass_id> subpasses;

	public:
		Offset2D offset = Offset2D { 0, 0 };
		Extent2D extent = Extent2D { 0, 0 };
	};
}