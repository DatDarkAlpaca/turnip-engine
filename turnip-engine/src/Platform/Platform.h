#pragma once
#include "Common.h"
#include "Core/Window/WindowProperties.h"

#if defined(TUR_WINDOWING_WIN32)
	#include "Platform/WIN32/Events_WIN32.h"
	#include "Platform/WIN32/Monitor_WIN32.h"
	#include "Platform/WIN32/Window_WIN32.h"
	#include "Platform/WIN32/Setup_WIN32.h"

	#include "Platform/WIN32/OpenGL/SetupGL.h"
	#include "Platform/WIN32/Vulkan/SetupVulkan.h"

#elif defined(TUR_WINDOWING_GLFW)
	#include <GLFW/glfw3.h>
 
	#include "Platform/GLFW/Events_GLFW.h"
	#include "Platform/GLFW/Monitor_GLFW.h"
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/Setup_GLFW.h"

	#include "Platform/GLFW/OpenGL/SetupGL.h"
	#include "Platform/GLFW/Vulkan/SetupVulkan.h"

#endif

namespace tur
{
	inline void RecreateWindow(Window* window)
	{
		WindowProperties windowProperties = window ? window->GetProperties() : WindowProperties{ };
		window->Initialize(windowProperties);
	}
}