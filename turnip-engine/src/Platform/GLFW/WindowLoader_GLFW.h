#pragma once
#include "pch.h"
#ifdef TUR_WINDOWING_GLFW
#include "Platform/GLFW/Window_GLFW.h"

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