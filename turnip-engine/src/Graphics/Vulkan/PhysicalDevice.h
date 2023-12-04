#pragma once
#include <string>
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	struct PhysicalDeviceOutput
	{
		vk::PhysicalDevice device;
		std::string deviceName;
	};

	class PhysicalDeviceSelector
	{
	public:
		PhysicalDeviceSelector()
		{

		}
	};
}