#pragma once
#include <numeric>
#include <limits>

#include "Type/Types.hpp"
#include "Common.hpp"

namespace tur
{
	using handle_type = uint32_t;

	constexpr handle_type invalid_handle = std::numeric_limits<handle_type>::max();
}