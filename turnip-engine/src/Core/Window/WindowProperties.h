#pragma once
#include <glm/glm.hpp>
#include <string>

namespace tur
{
	struct WindowProperties
	{
		glm::vec2 dimensions = glm::vec2(640, 480);
		glm::vec2 position = glm::vec2(100, 100);
		std::string title = "Window";
	};
}