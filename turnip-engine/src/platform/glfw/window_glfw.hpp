#pragma once
#include <GLFW/glfw3.h>

#include "core/event/events.hpp"
#include "core/config/window_configuration.hpp"

#include "platform/window.hpp"
#include "common.hpp"

namespace tur
{
	struct Window
	{
		GLFWwindow* window = nullptr;
		WindowProperties properties;

		struct Data
		{
			EventCallback eventCallback;
			bool framebufferResized = false;
		} data;
	};
}