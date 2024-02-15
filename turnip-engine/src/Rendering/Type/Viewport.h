#pragma once
#include <glm/glm.hpp>

namespace tur
{
	struct Viewport
	{
		glm::vec2 origin     = glm::vec2(0.f);
		glm::vec2 dimensions = glm::vec2(0.f);
		float minDepth = 0.f, maxDepth = 1.0f;
	};
}