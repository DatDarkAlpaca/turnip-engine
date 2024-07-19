#pragma once
#include "Common.hpp"
#include "Core/Window/WindowProperties.hpp"

#if defined(TUR_WINDOWING_WIN32)
	#include "Platform/WIN32/Setup_WIN32.hpp"
	#include "Platform/WIN32/Window_WIN32.hpp"
	#include "Platform/WIN32/Events_WIN32.hpp"
	#include "Platform/WIN32/Monitor_WIN32.hpp"

#elif defined(TUR_WINDOWING_GLFW) 
	#include "Platform/GLFW/Setup_GLFW.hpp"
	#include "Platform/GLFW/Window_GLFW.hpp"
	#include "Platform/GLFW/Events_GLFW.hpp"
	#include "Platform/GLFW/Monitor_GLFW.hpp"

	#include "Platform/GLFW/OpenGL/SetupOpenGL.hpp"
	#include "Platform/GLFW/Vulkan/SetupVulkan.hpp"
#endif