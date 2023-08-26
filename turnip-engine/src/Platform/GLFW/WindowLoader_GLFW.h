#pragma once
#ifdef TUR_WINDOWING_GLFW
#define GLFW_INCLUDE_NONE
#include "Platform/GLFW/Window_GLFW.h"
#include <GLFW/glfw3.h>
#include "Utils.h"

namespace tur
{
	class WindowLoader
	{
	public:
		bool Preload()
		{
			if (!glfwInit())
			{
				TUR_CORE_ERROR("Failed to initialize GLFW");
				return false;
			}

			return true;
		}
	};
}
#endif