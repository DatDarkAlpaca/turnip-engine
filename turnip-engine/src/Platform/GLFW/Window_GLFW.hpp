#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "common.hpp"
#include "platform/window.hpp"
#include "core/event/events.hpp"

namespace tur
{
	struct Window
	{
		GLFWwindow* window = nullptr;
		std::string title;
		glm::uvec2 size = glm::uvec2(640, 480);

		glm::uvec2 position = invalid_size;
		glm::uvec2 minSize  = invalid_size;
		glm::uvec2 maxSize  = invalid_size;

		struct Data
		{
			EventCallback eventCallback;
		} data;
	};
}