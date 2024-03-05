#pragma once
#include <GLFW/glfw3.h>

#include "Platform/GLFW/Window_GLFW.h"

namespace tur::platform
{
	inline void SwapBuffers(Window* window)
	{
		glfwSwapBuffers(std::any_cast<GLFWwindow*>(window->GetHandle()));
	}
}