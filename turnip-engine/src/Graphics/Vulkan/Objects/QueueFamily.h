#pragma once
#include <vulkan/vulkan.hpp>

#include "QueueOperation.h"
#include "Graphics/Vulkan/Constants.h"

namespace tur::vulkan
{
	struct QueueFamilyInformation
	{
		uint32_t familyIndex = vulkan::InvalidQueueIndex;
		uint32_t queueCount  = 0;
		QueueOperation supportedOperations = QueueOperation::NONE;
	};

	inline std::vector<QueueFamilyInformation> GetQueueFamilyInformation(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
	{
		std::vector<vk::QueueFamilyProperties> familyProperties = device.getQueueFamilyProperties();

		std::vector<QueueFamilyInformation> results;
		results.reserve(familyProperties.size());

		uint32_t queueIndex = 0;
		for (const auto& queueFamily : familyProperties)
		{
			QueueFamilyInformation queueFamilyEntry;

			queueFamilyEntry.familyIndex = queueIndex;
			queueFamilyEntry.queueCount = queueFamily.queueCount;

			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
				queueFamilyEntry.supportedOperations |= QueueOperation::GRAPHICS;

			if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
				queueFamilyEntry.supportedOperations |= QueueOperation::COMPUTE;

			if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer)
				queueFamilyEntry.supportedOperations |= QueueOperation::TRANSFER;

			if (queueFamily.queueFlags & vk::QueueFlagBits::eSparseBinding)
				queueFamilyEntry.supportedOperations |= QueueOperation::SPARSE_BINDING;

			if (device.getSurfaceSupportKHR(queueIndex, surface))
				queueFamilyEntry.supportedOperations |= QueueOperation::PRESENT;

			results.push_back(queueFamilyEntry);
		}

		return results;
	}

	inline bool GetQueueFamilySupports(const QueueFamilyInformation& queue, QueueOperation operation)
	{
		return uint32_t(queue.supportedOperations & operation);
	}

	inline bool GetQueueFamilySupports(const QueueFamilyInformation& queue, uint32_t operationFlags)
	{
		return uint32_t(queue.supportedOperations) & operationFlags;
	}
}