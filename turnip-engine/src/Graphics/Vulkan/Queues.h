#pragma once
#include <vulkan/vulkan.hpp>

namespace tur::vulkan
{
	enum QueueOperation : uint32_t
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

	inline QueueOperation operator|= (QueueOperation lhs, QueueOperation rhs)
	{
		return lhs | rhs;
	}

	struct QueueFamilyInformation
	{
		uint32_t familyIndex = InvalidQueueIndex;
		uint32_t queueCount = InvalidQueueIndex;
		QueueOperation supportedOperations = QueueOperation::NONE;
	};

	inline std::vector<QueueFamilyInformation> GetQueueInformation(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
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
		}

		return results;
	}

	inline bool GetQueueSupports(const QueueFamilyInformation& queue, QueueOperation operation)
	{
		return queue.supportedOperations & operation;
	}

	inline bool GetQueueSupports(const QueueFamilyInformation& queue, uint32_t operationFlags)
	{
		return queue.supportedOperations & operationFlags;
	}

	inline vk::DeviceQueueCreateInfo SelectQueue(uint32_t queueIndex, float priority = 1.0f)
	{
		return vk::DeviceQueueCreateInfo { vk::DeviceQueueCreateFlags(), queueIndex, 1, & priority };
	}
}