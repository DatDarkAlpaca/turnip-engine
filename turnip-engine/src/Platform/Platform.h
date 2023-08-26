#pragma once
#include "Common.h"

#ifdef TUR_WINDOWING_WINDOWS
	#include "Platform/Windows/Window_WIN32.h"
	#include "Platform/Windows/WindowLoader_WIN32.h"
	#include "Platform/Windows/OpenGL/OpenGLHandler_WIN32.h"
#endif

#ifdef TUR_WINDOWING_GLFW
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/WindowLoader_GLFW.h"
	#include "Platform/GLFW/OpenGL/OpenGLHandler_GLFW.h"
#endif