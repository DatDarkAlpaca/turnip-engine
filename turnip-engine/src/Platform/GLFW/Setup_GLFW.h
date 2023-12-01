#pragma once
#include <GLFW/glfw3.h>
#include "Core/Logger/Logger.h"

namespace
{
	inline void GLFWErrorCallback(int errorCode, const char* description)
	{
		TUR_LOG_ERROR("[GLFW]: {} [{}]", description, errorCode);
	}
}

namespace tur
{
	inline void PlatformSetup()
	{
		glfwSetErrorCallback(GLFWErrorCallback);
		if (!glfwInit())
			TUR_LOG_CRITICAL("Failed to initialize GLFW. Perhaps it has already been initialized before?");
	}
}