#pragma once
#include <optional>
#include <unordered_set>
#include "Graphics/Types/Types.h"

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

		DataFormat imageFormat = DataFormat::R8_G8_B8_A8_UNORM;
		ImageLayout initialLayout = ImageLayout::UNDEFINED;
		ImageLayout finalLayout = ImageLayout::PRESENT_SRC;
		ImageLayout attachmentLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL;
	};

	struct RenderpassDescriptor
	{
		using subpass_id = uint32_t;
		using attachment_id = uint32_t;

	public:
		uint32_t AddAttachment(const RenderpassAttachment& attachment)
		{
			attachments.push_back(attachment);
			return defaultSwapchainAttachment ? (uint32_t)attachments.size() : (uint32_t)attachments.size() - 1;
		}

		uint32_t AddSubpass()
		{
			subpasses.push_back({});
			return defaultSwapchainAttachment ? (uint32_t)subpasses.size() : (uint32_t)subpasses.size() - 1;
		}

		void AddSubpassMapping(subpass_id subpassIndex, attachment_id attachmentIndex, AttachmentType type = AttachmentType::COLOR)
		{
			TUR_ASSERT(subpassIndex < subpasses.size(), "Invalid subpass index");
			subpasses[subpassIndex].insert({ type, attachmentIndex });
		}

	public:
		std::optional<uint32_t> GetMainSubpass() const
		{
			if (defaultSwapchainAttachment)
				return 0;

			return std::nullopt;
		}

		std::optional<uint32_t> GetSwapchainAttachmentIndex() const
		{
			if (defaultSwapchainAttachment)
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
		bool defaultSwapchainAttachment = true;
		std::vector<RenderpassAttachment> attachments;
		std::vector<std::unordered_set<std::pair<AttachmentType, attachment_id>, attachment_pair_hash>> subpasses;
	};

	class IRenderpass
	{
	public:
		virtual ~IRenderpass() = default;
	};
}