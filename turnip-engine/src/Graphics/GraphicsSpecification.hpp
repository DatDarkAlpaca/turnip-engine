#pragma once
#include <numeric>
#include "GraphicsAPI.hpp"

namespace tur
{
	struct GraphicsSpecification
	{
		GraphicsAPI api = GraphicsAPI::UNKNOWN;
		uint16_t major = 0, minor = 0, patch = 0, variant = 0;
	};
}