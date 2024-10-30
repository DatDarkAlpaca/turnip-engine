#pragma once
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <string>

#include "Common.hpp"

namespace tur
{
	struct WindowProperties
	{
		enum Position : int { DEFAULT = -1 };
	
	public:
		std::string windowTitle = "TurnipEngine v1.0";
		glm::vec2 dimensions = glm::vec2(640, 480);
		glm::vec2 position = glm::vec2(Position::DEFAULT, Position::DEFAULT);
		glm::vec2 minSize = glm::vec2(0), maxSize = dimensions;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(WindowProperties, windowTitle,
			dimensions, position, minSize, maxSize);
	};
}