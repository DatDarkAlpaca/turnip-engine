#pragma once
#include <vulkan/vulkan.hpp>
#include "queue_usage.hpp"

namespace tur::vulkan
{
	struct QueueFamily
	{
		u32 index;
		QueueUsage supportedUsage;
	};

	inline QueueUsage get_queue_family_supported_features(const vk::PhysicalDevice& device, VkSurfaceKHR surface)
	{
		u32 support;
		
		u32 queueIndex = 0;
		for (const auto& queueFamily : device.getQueueFamilyProperties())
		{
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
				support |= static_cast<u32>(QueueUsage::GRAPHICS);

			if (device.getSurfaceSupportKHR(queueIndex, surface))
				support |= static_cast<u32>(QueueUsage::PRESENT);

			if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
				support |= static_cast<u32>(QueueUsage::COMPUTE);

			if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer)
				support |= static_cast<u32>(QueueUsage::TRANSFER);

			if (queueFamily.queueFlags & vk::QueueFlagBits::eSparseBinding)
				support |= static_cast<u32>(QueueUsage::SPARSE_BINDING);

			++queueIndex;
		}

		return static_cast<QueueUsage>(support);
	}

	inline std::vector<QueueFamily> get_queue_families(const vk::PhysicalDevice& device, VkSurfaceKHR surface)
	{
		std::vector<QueueFamily> queueFamilies;
		
		u32 queueIndex = 0;
		for (const auto& queueFamily : device.getQueueFamilyProperties())
		{
			u32 support;

			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				support |= static_cast<u32>(QueueUsage::GRAPHICS);
				support |= static_cast<u32>(QueueUsage::TRANSFER);
			}

			if (device.getSurfaceSupportKHR(queueIndex, surface))
				support |= static_cast<u32>(QueueUsage::TRANSFER);

			if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
			{
				support |= static_cast<u32>(QueueUsage::COMPUTE);
				support |= static_cast<u32>(QueueUsage::TRANSFER);
			}

			if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer)
				support |= static_cast<u32>(QueueUsage::TRANSFER);

			if (queueFamily.queueFlags & vk::QueueFlagBits::eSparseBinding)
				support |= static_cast<u32>(QueueUsage::SPARSE_BINDING);

			queueFamilies.push_back({ queueIndex, static_cast<QueueUsage>(support) });
			++queueIndex;
		}

		return queueFamilies;
	}
}