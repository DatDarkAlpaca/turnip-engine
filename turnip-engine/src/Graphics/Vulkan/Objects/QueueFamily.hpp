#pragma once
#include <vulkan/vulkan.hpp>
#include "Graphics/Vulkan/CommonVulkan.hpp"

namespace tur::vulkan
{
	enum class QueueOperation : uint32_t
	{
		NONE			= 0,
		GRAPHICS		= 1 << 0,
		PRESENT			= 1 << 1,
		COMPUTE			= 1 << 2,
		TRANSFER		= 1 << 3,
		SPARSE_BINDING  = 1 << 4
	};

	inline QueueOperation operator| (QueueOperation lhs, QueueOperation rhs)
	{
		return static_cast<QueueOperation>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}

	inline QueueOperation operator& (QueueOperation lhs, QueueOperation rhs)
	{
		return static_cast<QueueOperation>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}

	inline QueueOperation& operator|= (QueueOperation& lhs, QueueOperation rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	struct QueueFamilyInformation
	{
		uint32_t familyIndex = invalid_queue_index;
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