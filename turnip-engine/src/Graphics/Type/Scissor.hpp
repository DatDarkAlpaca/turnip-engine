#pragma once
#include <glm/glm.hpp>

namespace tur
{
	struct Scissor
	{
		glm::uvec2 offset = glm::uvec2(0);
		glm::uvec2 size   = glm::uvec2(0);
	};
}