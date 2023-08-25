#pragma once
#include "Common.h"

#ifdef TUR_WINDOWING_WINDOWS

#endif

#ifdef TUR_WINDOWING_GLFW
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/WindowLoader_GLFW.h"
#endif