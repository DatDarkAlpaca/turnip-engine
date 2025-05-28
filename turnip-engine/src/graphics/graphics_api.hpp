#pragma once
#include <string>
#include <string_view>

namespace tur
{
	enum class GraphicsAPI
	{
		UNKNOWN = 0,
		OPENGL,
		VULKAN
	};

	inline std::string_view get_graphics_api_name(GraphicsAPI api)
	{
		switch (api)
		{
			case GraphicsAPI::OPENGL:
				return "OPENGL";

			case GraphicsAPI::VULKAN:
				return "VULKAN";

			default:
				return "UNKNOWN";
		}
	}

	inline GraphicsAPI get_graphics_api_from_string(const std::string& graphicsAPI)
	{
		if (graphicsAPI == "OPENGL")
			return GraphicsAPI::OPENGL;

		if (graphicsAPI == "VULKAN")
			return GraphicsAPI::VULKAN;

		return GraphicsAPI::UNKNOWN;
	}
}