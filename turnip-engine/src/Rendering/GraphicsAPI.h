#pragma once
#include <numeric>

namespace tur
{
	enum class GraphicsAPI
	{
		NONE = 0,
		OPENGL,
		VULKAN
	};

	struct GraphicsSpecification
	{
		GraphicsAPI api;
		uint16_t major = 0, minor = 0, patch = 0, detail = 0;
	};
}