#pragma once
#include "Rendering/GraphicsAPI.h"
#include "Platform/GLFW/Window_GLFW.h"

namespace tur::platform
{
	void SetupOpenGL(Window& window, const GraphicsSpecification& specification);
}