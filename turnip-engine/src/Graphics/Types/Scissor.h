#pragma once
#include <glm/glm.hpp>

namespace tur
{
	struct Scissor
	{
		glm::vec2 offset = glm::vec2(0.f);
		glm::vec2 size   = glm::vec2(0.f);
	};
}