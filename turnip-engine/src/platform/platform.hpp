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

	inline void open_in_explorer(const std::filesystem::path& filepath)
	{
#ifdef TUR_PLATFORM_WIN32
		std::string command = "explorer " + filepath.parent_path().string();
		std::system(command.c_str());
#endif

#ifdef TUR_PLATFORM_LINUX
		// Also handle nautilus, gnome, etc
		std::string command = "xdg-open " + filepath.string();
		std::system(command.c_str());
#endif

#ifdef TUR_PLATFORM_MACOS
		// If that runs that's a miracle
		std::string command = "open " + filepath.string();
		std::system(command.c_str());
#endif
	}
}