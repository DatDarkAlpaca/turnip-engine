#pragma once
#include "Graphics/GraphicsSpecification.hpp"
#include "Platform/GLFW/Window_GLFW.hpp"

namespace tur::platform::gl
{
	void SetupOpenGLWindowing(Window& window, const WindowProperties&, const GraphicsSpecification&);

	void SwapBuffers(Window* window);
}