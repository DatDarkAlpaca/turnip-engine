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

	struct GraphicsOptions
	{
		bool usingBottomLeftOrigin = true;
	};

	struct GraphicsSpecification
	{
		GraphicsAPI api;
		uint16_t major = 0, minor = 0, patch = 0, variant = 0;
		GraphicsOptions options = {};
	};
}