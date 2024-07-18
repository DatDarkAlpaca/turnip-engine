#pragma once

namespace tur
{
	enum class AttachmentLoadOperation
	{
		NONE,
		LOAD,
		CLEAR,
		DONT_CARE
	};

	enum class AttachmentStoreOperation
	{
		NONE,
		STORE,
		DONT_CARE,
	};

	struct AttachmentOperations
	{
		AttachmentLoadOperation  attachmentLoad  = AttachmentLoadOperation::CLEAR;
		AttachmentStoreOperation attachmentStore = AttachmentStoreOperation::STORE;
		AttachmentLoadOperation  stencilLoad     = AttachmentLoadOperation::DONT_CARE;
		AttachmentStoreOperation stencilStore    = AttachmentStoreOperation::DONT_CARE;
	};
}