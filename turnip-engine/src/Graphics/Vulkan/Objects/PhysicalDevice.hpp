#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct PhysicalDeviceObject
	{
		vk::PhysicalDevice physicalDevice;
	};
}