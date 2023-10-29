#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Common.h"

namespace tur
{
	enum class WindowPosition : int { DEFAULT = -1 };

	struct WindowProperties
	{
		std::string windowTitle = "TurnipEngine v1.0";
		glm::vec2 dimensions = glm::vec2(640, 480);
		glm::vec2 position = glm::vec2(WindowPosition::DEFAULT, WindowPosition::DEFAULT);
		glm::vec2 minSize = glm::vec2(0), maxSize = glm::vec2(0);
	};
}