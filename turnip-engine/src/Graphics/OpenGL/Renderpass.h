#pragma once
#include "Rendering/Type/Types.h"
#include "Rendering/Resource/Renderpass.h"

namespace tur::gl
{
	struct Renderpass
	{
		uint32_t framebufferID;
		Extent extent;
		Offset offset;
	};

	inline void SetupRenderpass(Renderpass& renderpass, const RenderpassDescriptor& descriptor)
	{
		renderpass.framebufferID = 0;
		renderpass.extent = descriptor.extent;
		renderpass.offset = descriptor.offset;

		if (!descriptor.defaultSwapchainAttachment)
			glGenFramebuffers(1, &renderpass.framebufferID);
		
		glBindFramebuffer(GL_FRAMEBUFFER, renderpass.framebufferID);

		auto& attachments = descriptor.subpasses;
		auto& subpasses = descriptor.subpasses;
	}
}