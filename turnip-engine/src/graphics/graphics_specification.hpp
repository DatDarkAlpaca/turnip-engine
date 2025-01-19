#pragma once
#include "common.hpp"
#include "graphics_api.hpp"

namespace tur
{
	struct GraphicsSpecification
	{
		GraphicsAPI api = GraphicsAPI::UNKNOWN;
		u8 major = 0;
		u8 minor = 0;
		u8 patch = 0;
		u8 variant = 0;
	};
}