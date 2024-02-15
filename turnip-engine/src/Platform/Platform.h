#pragma once
#include "Common.h"
#include "Core/Window/WindowProperties.h"

// TODO: identify if the system supports the respective graphics api
#include "OpenGL/BufferGL.h"
#include "OpenGL/Diagnostics.h"

#if defined(TUR_WINDOWING_WIN32)
	#include "Platform/WIN32/Events_WIN32.h"
	#include "Platform/WIN32/Monitor_WIN32.h"
	#include "Platform/WIN32/Window_WIN32.h"
	#include "Platform/WIN32/Setup_WIN32.h"

#elif defined(TUR_WINDOWING_GLFW)
	#include <GLFW/glfw3.h>
 
	#include "Platform/GLFW/Events_GLFW.h"
	#include "Platform/GLFW/Monitor_GLFW.h"
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/Setup_GLFW.h"

	#include "Platform/GLFW/OpenGL/PresentGL.h"
	#include "Platform/GLFW/OpenGL/SetupOpenGL.h"
#endif