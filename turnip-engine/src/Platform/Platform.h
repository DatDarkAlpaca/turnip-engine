#pragma once
#include "Common.h"

#ifdef TUR_WINDOWING_WINDOWS
	#include "Platform/Windows/Window_WIN32.h"
	#include "Platform/Windows/WindowLoader_WIN32.h"

namespace tur
{
	using WindowLoader = WindowLoader_WIN32;
	using Window = Window_WIN32;
}
#endif

#ifdef TUR_WINDOWING_GLFW
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/WindowLoader_GLFW.h"

namespace tur
{
	using WindowLoader = WindowLoader_GLFW;
	using Window = Window_GLFW;
}

#endif