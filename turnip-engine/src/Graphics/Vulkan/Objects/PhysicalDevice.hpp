#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueFamily.h"

namespace tur::vulkan
{
	enum class PhysicalDeviceType
	{
		NONE				= 0xFFFFFFF,
		IntegratedGpu		= VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
		DiscreteGpu			= VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
		VirtualGpu			= VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
		CPU					= VK_PHYSICAL_DEVICE_TYPE_CPU,
		Other				= VK_PHYSICAL_DEVICE_TYPE_OTHER
	};

	struct PhysicalDevice
	{
		vk::PhysicalDevice physicalDevice;
		std::vector<QueueFamilyInformation> queueFamilyInformation;
	};
}