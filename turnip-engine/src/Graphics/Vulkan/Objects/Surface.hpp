#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct SurfaceObject
	{
	public:
		SurfaceObject(VkSurfaceKHR surface)
			: surface(surface) { }

		explicit SurfaceObject() = default;

	public:
		VkSurfaceKHR surface;
	};
}