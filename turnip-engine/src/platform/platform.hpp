#pragma once

#include "window.hpp"
#include "platform/opengl_context.hpp"
#include "platform/vulkan_context.hpp"

#if defined(TUR_PLATFORM_WIN32) || defined(TUR_PLATFORM_LINUX) || defined(TUR_PLATFORM_MACOS)
	#include "glfw/window_glfw.hpp"
	#include "glfw/events_glfw.hpp"
#endif

namespace tur
{
	inline int run_command(const std::string& command)
	{
		return std::system(command.c_str());
	}
}