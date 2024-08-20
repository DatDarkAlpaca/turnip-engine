#pragma once
#include "CommonGraphics.hpp"
#include "Framebuffer.hpp"

namespace tur
{
	using renderpass_handle = handle_type;

	struct RenderpassDescriptor
	{
		FramebufferDescriptor frambufferDescriptor;
	};
}