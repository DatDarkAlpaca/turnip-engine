#pragma once
#include <numeric>
#include <glm/glm.hpp>
#include "utils/color.hpp"

namespace tur
{
	enum class ClearFlags : u32
	{
		COLOR	= 1 << 0,
		DEPTH   = 1 << 1,
		STENCIL = 1 << 1,
	};

	struct ClearValue
	{
		Color color = color::White;
		float depth = 1.0f;
		u32 stencil = 0;
	};
}