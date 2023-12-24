#pragma once
#include <glm/glm.hpp>

namespace tur
{
	struct Scissor
	{
		glm::uvec2 offset = glm::vec2(0);
		glm::uvec2 size   = glm::vec2(0);
	};
}