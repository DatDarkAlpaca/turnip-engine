#pragma once
#include <numeric>
#include <limits>

namespace tur
{
	using GLID = uint32_t;
	constexpr GLID InvalidGLID = std::numeric_limits<GLID>::max();
}