#pragma once
#include "pch.h"

#ifdef TUR_PLATFORM_WINDOWS
	#include "Platform/Windows/WIN32_Window.h"
#else
	#include "Platform/GLFW/GLFW_Window.h"
#endif

namespace tur
{
#ifdef TUR_PLATFORM_WINDOWS
	using Window = WIN32_Window;
#else
	using Window = GLFW_Window;
#endif
}
