#pragma once
#include "Core/Logger/Logger.h"

#include <GLFW/glfw3.h>

namespace
{
	inline void GLFWErrorCallback(int errorCode, const char* description)
	{
		TUR_LOG_ERROR("[GLFW]: {} [{}]", description, errorCode);
	}
}

namespace tur::platform
{
	inline void InitializePlatform()
	{
		glfwSetErrorCallback(GLFWErrorCallback);
		if (!glfwInit())
			TUR_LOG_CRITICAL("Failed to initialize the platform system.");
	}
}