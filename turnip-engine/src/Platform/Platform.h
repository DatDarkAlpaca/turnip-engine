#pragma once
#include "Common.h"
#include "Core/Window/WindowProperties.h"

#ifdef TUR_PLATFORM_WIN32
	#include "Platform/GLFW/Setup_GLFW.h"
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/Monitor_GLFW.h"
	#include "Platform/GLFW/Events_GLFW.h"
	#include "Platform/GLFW/EventDebug_GLFW.h"

#elif defined(TUR_PLATFORM_LINUX)
	#include "Platform/GLFW/Setup_GLFW.h"
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/Monitor_GLFW.h"
	#include "Platform/GLFW/Events_GLFW.h"
	#include "Platform/GLFW/EventDebug_GLFW.h"
#endif