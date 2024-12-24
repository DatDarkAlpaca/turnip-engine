#pragma once
#include <vulkan/vulkan.hpp>
#include "Graphics/Vulkan/CommonVulkan.hpp"
#include "QueueFamily.hpp"

namespace tur::vulkan
{
	struct DeviceQueueData
	{
		vk::Queue queue;
		uint32_t familyIndex;
		QueueOperation operation;
	};

	struct DeviceQueues
	{
	public:
		inline vk::Queue Get(QueueOperation operation) const
		{
			for (const auto& [queue, _, supportedOperations] : queues)
			{
				if ((uint32_t)(supportedOperations & operation))
					return queue;
			}

			return nullptr;
		}

		inline uint32_t GetFamily(QueueOperation operation) const
		{
			for (const auto& [_, index, supportedOperations] : queues)
			{
				if ((uint32_t)(supportedOperations & operation))
					return index;
			}

			return invalid_queue_index;
		}

		inline void Add(vk::Queue queue, uint32_t index, QueueOperation operation)
		{
			queues.push_back({ queue, index, operation });
		}

	public:
		std::vector<DeviceQueueData> queues;
	};
}