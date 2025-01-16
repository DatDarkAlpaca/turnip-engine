#pragma once
#include "graphics/graphics.hpp"
#include "platform/window.hpp"

namespace tur
{
	struct Swapbuffer
	{
		Window* window;
	};

	extern void initialize_opengl_windowing(Window* window, const GraphicsSpecification& specification);
}