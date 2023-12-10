#include "pch.h"
#include "PhysicalDevice.h"

namespace tur::vulkan
{
	vk::PhysicalDevice DefaultPhysicalDeviceSelector(const PhysicalDeviceSelector& deviceSelector, const std::vector<vk::PhysicalDevice>& physicalDevices)
	{
		const auto& instanceOutput = deviceSelector.GetInstanceOutput();
		const auto& surface = deviceSelector.GetSurface();
		const auto& requestedExtensions = deviceSelector.GetRequestedExtensions();

		using score = uint32_t;

		std::vector<score> scoreList(physicalDevices.size());
		std::fill(scoreList.begin(), scoreList.end(), -1);

		// Get device with most memory:
		size_t maxMemoryElementIndex = 0;
		auto maxMemoryElement = std::max_element(physicalDevices.begin(), physicalDevices.end(), [](vk::PhysicalDevice lhs, vk::PhysicalDevice rhs)
			{
				auto memoryLHS = lhs.getMemoryProperties();
				auto memoryRHS = rhs.getMemoryProperties();

				size_t heapSizeLHS = 0, heapSizeRHS = 0;
				for (size_t i = 0; i < memoryLHS.memoryHeapCount; ++i)
				{
					if (memoryLHS.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eDeviceLocal)
						heapSizeLHS = memoryLHS.memoryHeaps[i].size;
				}

				for (size_t i = 0; i < memoryRHS.memoryHeapCount; ++i)
				{
					if (memoryRHS.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eDeviceLocal)
						heapSizeRHS = memoryRHS.memoryHeaps[i].size;
				}

				return (heapSizeLHS > heapSizeRHS);
			});
		maxMemoryElementIndex = std::distance(physicalDevices.begin(), maxMemoryElement);

		for (size_t i = 0; i < physicalDevices.size(); ++i)
		{
			auto& device = physicalDevices[i];

			auto extensionProperties = device.enumerateDeviceExtensionProperties();
			auto deviceProperties = device.getProperties();
			auto memoryProperties = device.getMemoryProperties();

			// Required Extensions:
			bool supported;
			for (const auto& extension : requestedExtensions)
			{
				supported = false;
				for (const auto& properties : extensionProperties)
				{
					if (properties.extensionName == extension)
					{
						supported = true;
						scoreList[i] += 1;
						break;
					}
				}
			}

			if (!supported)
				continue;
			else
				scoreList[i] += 1;

			// Required Queue Support:
			if (instanceOutput.enablePresentation)
			{
				// Queues:
				auto queuesSupported = GetQueueInformation(device, surface);
				bool foundSupported = false;
				for (const auto& queue : queuesSupported)
				{
					if (GetQueueSupports(queue, QueueOperation::GRAPHICS & QueueOperation::PRESENT))
					{
						scoreList[i] += 1;
						foundSupported = true;
						break;
					}
				}

				if (!foundSupported)
					continue;
			}

			// Favors discrete GPUs:
			if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
				scoreList[i] += 2;
		}

		// Favors most memory:
		scoreList[maxMemoryElementIndex] += 1;

		size_t winnerIndex = std::distance(scoreList.begin(), std::max_element(scoreList.begin(), scoreList.end()));

		// probably doesn't work:
		if (scoreList[winnerIndex] == -1)
			TUR_LOG_CRITICAL("None of the available physical devices support the features requested.");

		return physicalDevices[winnerIndex];
	}
}