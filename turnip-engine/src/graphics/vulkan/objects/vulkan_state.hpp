#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include "graphics/vulkan/objects/queue_list.hpp"

namespace tur::vulkan
{
	struct VulkanState
	{
		// Instance:
		u32 apiVersion;
		vk::Instance instance;
		vk::DebugUtilsMessengerEXT debugMessenger;
		vk::DispatchLoaderDynamic DLDI;
		bool requiresDrawing;
		bool validationEnabled;

		// Surface:
		VkSurfaceKHR surface;

		// Physical Device:
		vk::PhysicalDevice physicalDevice;

		// Logical Device:
		vk::Device logicalDevice;
		QueueList queueList;

		// VMA:
		VmaAllocator vmaAllocator;
	};
}