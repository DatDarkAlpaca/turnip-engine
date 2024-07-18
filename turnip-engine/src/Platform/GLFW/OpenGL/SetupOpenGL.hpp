#pragma once
#include "Graphics/GraphicsSpecification.h"
#include "Platform/GLFW/Window_GLFW.h"

namespace tur::platform::gl
{
	void SetupOpenGLWindowing(Window& window, const WindowProperties&, const GraphicsSpecification&);

	void SwapBuffers(Window* window);
}