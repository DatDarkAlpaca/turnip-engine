#pragma once
#include <numeric>
#include <glm/glm.hpp>

namespace tur
{
	enum class ClearFlags : uint32_t
	{
		COLOR	= 1 << 0,
		DEPTH   = 1 << 1,
		STENCIL = 1 << 1,
	};

	struct ClearValue
	{
		glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.0f);
		float depth = 1.0f;
		uint32_t stencil = 0;
	};
}