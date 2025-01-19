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
		u8 major = 0;
		u8 minor = 0;
		u8 patch = 0;
		u8 variant = 0;
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