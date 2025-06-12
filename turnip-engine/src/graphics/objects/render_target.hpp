#pragma once
#include "texture.hpp"

namespace tur
{
	using render_target_handle = handle_type;

	struct RenderTargetDescriptor
	{
		u32 width;
		u32 height;

		std::vector<texture_handle> colorAttachments;
		texture_handle depthAttachment = invalid_handle;
	};
}