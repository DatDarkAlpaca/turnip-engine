#pragma once
#include <optional>
#include <unordered_set>

#include "Common.h"
#include "Rendering/Type/Types.h"

namespace tur
{
	enum class AttachmentType
	{
		NONE = 0,
		INPUT,
		COLOR,
		RESOLVE,
		DEPTH_STENCIL
	};

	struct RenderpassAttachment
	{
		uint32_t sampleCount = 1;
		AttachmentLoadOperation loadOperation = AttachmentLoadOperation::CLEAR;
		AttachmentStoreOperation storeOperation = AttachmentStoreOperation::STORE;
		AttachmentLoadOperation stencilLoadOperation = AttachmentLoadOperation::DONT_CARE;
		AttachmentStoreOperation stencilStoreOperation = AttachmentStoreOperation::DONT_CARE;

		DataFormat imageFormat = DataFormat::B8_G8_R8_A8_UNORM;
		ImageLayout initialLayout = ImageLayout::UNDEFINED;
		ImageLayout finalLayout = ImageLayout::PRESENT_SRC;
		ImageLayout attachmentLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL;
	};

	struct RenderpassDescriptor
	{
		using subpass_id = uint32_t;
		using attachment_id = uint32_t;

	public:
		static RenderpassDescriptor CreateDefaultRenderpass()
		{
			RenderpassDescriptor descriptor;
			descriptor.defaultSwapchainAttachment = true;
			descriptor.pipelineType = PipelineType::GRAPHICS;

			auto attachmentID = descriptor.AddAttachment(RenderpassAttachment{});
			auto subpassID = descriptor.AddSubpass();
			descriptor.AddSubpassMapping(subpassID, attachmentID, AttachmentType::COLOR);

			return descriptor;
		}

	public:
		uint32_t AddAttachment(const RenderpassAttachment& attachment)
		{
			if (defaultSwapchainAttachment)
			{
				if(attachments.size() == 0)
					attachments.push_back(attachment);
				else
					attachments[0] = attachment;
			}
			else
				attachments.push_back(attachment);

			return (uint32_t)attachments.size() - 1;
		}

		uint32_t AddSubpass()
		{
			if (defaultSwapchainAttachment)
			{
				if (subpasses.size() == 0)
					subpasses.push_back({});
				else
					subpasses[0] = {};
			}
			else
				subpasses.push_back({});

			return (uint32_t)subpasses.size() - 1;
		}

		void AddSubpassMapping(subpass_id subpassIndex, attachment_id attachmentIndex, AttachmentType type = AttachmentType::COLOR)
		{
			TUR_ASSERT(subpassIndex < subpasses.size(), "Invalid subpass index");
			subpasses[subpassIndex].insert({ type, attachmentIndex });
		}

	public:
		std::optional<uint32_t> GetMainSubpass() const
		{
			if (defaultSwapchainAttachment && attachments.size() > 0)
				return 0;

			return std::nullopt;
		}

		std::optional<uint32_t> GetSwapchainAttachmentIndex() const
		{
			if (defaultSwapchainAttachment && subpasses.size() > 0)
				return 0;

			return std::nullopt;
		}

	private:
		struct attachment_pair_hash
		{
			inline std::size_t operator() (const std::pair<AttachmentType, attachment_id>& value) const
			{
				// https://stackoverflow.com/questions/15160889/how-can-i-make-an-unordered-set-of-pairs-of-integers-in-c
				return static_cast<std::size_t>(static_cast<uint32_t>(value.first) * 31 + value.second);
			}
		};

	public:
		bool defaultSwapchainAttachment = false;
		PipelineType pipelineType = PipelineType::GRAPHICS;

	public:
		std::vector<RenderpassAttachment> attachments;
		std::vector<std::unordered_set<std::pair<AttachmentType, attachment_id>, attachment_pair_hash>> subpasses;

	public:
		Offset offset = Offset{ 0,0 };
		Extent extent = { 0, 0 };
	};

	enum class RenderpassHandle : uint32_t { SCREEN_PASS = 0, INVALID = InvalidHandle };
}