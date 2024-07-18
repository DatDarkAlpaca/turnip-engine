#pragma once
#include <numeric>

namespace tur
{
	enum class CullMode : uint32_t
	{
		NONE				= 1 << 0,
		FRONT				= 1 << 1,
		BACK				= 1 << 2,
		FRONT_AND_BACK		= 1 << 3
	};

	inline uint32_t operator& (CullMode lhs, CullMode rhs)
	{
		return static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs);
	}

	inline uint32_t operator| (CullMode lhs, CullMode rhs)
	{
		return static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs);
	}
}