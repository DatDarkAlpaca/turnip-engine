#pragma once
#include "Common.h"
#include "Core/Window/WindowProperties.h"

#if defined(TUR_WINDOWING_WIN32)
	#include "Platform/WIN32/Events_WIN32.h"
	#include "Platform/WIN32/Monitor_WIN32.h"
	#include "Platform/WIN32/Window_WIN32.h"
	#include "Platform/WIN32/Setup_WIN32.h"

#elif defined(TUR_WINDOWING_GLFW) 
	#include "Platform/GLFW/Events_GLFW.h"
	#include "Platform/GLFW/Monitor_GLFW.h"
	#include "Platform/GLFW/Window_GLFW.h"
	#include "Platform/GLFW/Setup_GLFW.h"

	#include "Platform/GLFW/OpenGL/PresentGL.h"
	#include "Platform/GLFW/OpenGL/SetupOpenGL.h"
	#include "Platform/GLFW/Vulkan/SetupVulkan.h"

	#include "Platform/GLFW/ImGUI/ImGUI.h"
	#include "Platform/GLFW/ImGUI/ViewImGUI_GLFW.h"
#endif