#pragma once
#include "graphics/graphics_specification.hpp"
#include "platform/window.hpp"

namespace tur
{
	extern void initialize_opengl_windowing(Window* window, const WindowProperties& properties, const GraphicsSpecification& specification);

	extern void initialize_opengl_gui(Window* window);

	extern void present_opengl_window(Window* window);
}