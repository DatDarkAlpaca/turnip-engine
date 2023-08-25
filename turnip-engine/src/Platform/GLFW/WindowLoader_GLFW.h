#pragma once
#include "pch.h"
#ifdef TUR_WINDOWING_GLFW
#include "Core/Window/BaseWindowLoader.h"

namespace tur
{
	class WindowLoader_GLFW : public BaseWindowLoader<WindowLoader_GLFW>
	{
	public:
		bool Preload()
		{
			if (!glfwInit())
				return s_IsGLFWInitialized;

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			s_IsGLFWInitialized = true;
			return true;
		}

		bool PostLoad() { return true; }

	private:
		static inline bool s_IsGLFWInitialized = false;
	};
}
#endif