#pragma once
#include "Common.h"
#include "Core/Window/WindowProperties.h"

#ifdef TUR_WINDOWING_GLFW
	#include "Platform/GLFW/EventDebug_GLFW.h"
	#include "Platform/GLFW/Events_GLFW.h"
	#include "Platform/GLFW/Monitor_GLFW.h"
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/Setup_GLFW.h"

	#include "Platform/GLFW/OpenGL/SetupGL.h"

	#include <vulkan/vulkan.h>
	#include "Platform/GLFW/Vulkan/SetupVulkan.h"

namespace tur
{
	using Window = WindowBase<WindowGLFW>;

	inline void RecreateWindow(Window* window)
	{
		WindowProperties windowProperties = window ? window->GetProperties() : WindowProperties { };
		window->Initialize(windowProperties);
	}
}

#endif