#pragma once
#include "Rendering/GraphicsAPI.h"
#include "Platform/Platform.h"

namespace tur::platform
{
	void SetupOpenGL(Window& window, const GraphicsSpecification& specification);
}