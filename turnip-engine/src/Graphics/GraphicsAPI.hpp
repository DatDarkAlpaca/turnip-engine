#pragma once
#include <numeric>
#include <string>

namespace tur
{
	enum class GraphicsAPI : uint32_t
	{
		UNKNOWN = 0,
		OPENGL,
		VULKAN
	};

	inline std::string_view GetGraphicsAPIString(GraphicsAPI api)
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

	inline GraphicsAPI GetGraphicsAPIFromString(const std::string& graphicsAPI)
	{
		if (graphicsAPI == "OPENGL")
			return GraphicsAPI::OPENGL;

		if (graphicsAPI == "VULKAN")
			return GraphicsAPI::VULKAN;

		return GraphicsAPI::UNKNOWN;
	}
}