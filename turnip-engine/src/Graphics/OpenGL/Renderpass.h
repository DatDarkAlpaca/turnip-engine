#pragma once
#include "Rendering/Resource/Renderpass.h"

namespace tur::gl
{
	struct Renderpass
	{
		uint32_t framebufferID;
	};

	inline void SetupRenderpass(Renderpass& renderpass, const RenderpassDescriptor& descriptor)
	{
		renderpass.framebufferID = 0;
		if (!descriptor.defaultSwapchainAttachment)
			glGenFramebuffers(1, &renderpass.framebufferID);
		
		glBindFramebuffer(GL_FRAMEBUFFER, renderpass.framebufferID);

		auto& attachments = descriptor.subpasses;
		auto& subpasses = descriptor.subpasses;
	}
}