#pragma once
#include "graphics/graphics_specification.hpp"
#include "platform/window.hpp"

namespace tur::gl
{
	extern void initialize_opengl_windowing(Window* window, const WindowProperties& properties, const GraphicsSpecification& specification);
	extern void initialize_opengl_gui(Window* window);

	extern void begin_opengl_frame();
	extern void render_opengl_frame();
	extern void end_opengl_frame();

	extern void present_opengl_window(Window* window);
}