#pragma once
#include "common.hpp"

namespace tur
{
	enum class GraphicsAPI
	{
		UNKNOWN = 0,
		OPENGL,
	};

	struct GraphicsSpecification
	{
		GraphicsAPI api = GraphicsAPI::UNKNOWN;
		uint8_t major = 0;
		uint8_t minor = 0;
		uint8_t patch = 0;
		uint8_t variant = 0;
	};

	inline std::string_view get_graphics_api_name(GraphicsAPI api)
	{
		switch (api)
		{
			case GraphicsAPI::OPENGL:
				return "OPENGL";

			default:
				return "UNKNOWN";
		}
	}

	inline GraphicsAPI get_graphics_api_from_string(const std::string& graphicsAPI)
	{
		if (graphicsAPI == "OPENGL")
			return GraphicsAPI::OPENGL;

		return GraphicsAPI::UNKNOWN;
	}
}