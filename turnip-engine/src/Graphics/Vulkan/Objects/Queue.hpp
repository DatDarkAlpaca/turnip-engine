#pragma once
#include <vulkan/vulkan.hpp>

#include "Common.hpp"
#include "QueueOperation.hpp"

namespace tur::vulkan
{
	struct QueueCluster
	{
	public:
		struct QueueData
		{
			uint32_t familyIndex;
			vk::Queue queue;
			QueueOperation operation;
		};

	public:
		inline vk::Queue Get(QueueOperation operation) const
		{
			for (const auto& [_, queue, supportedOperations] : queues)
			{
				if ((uint32_t)(supportedOperations & operation))
					return queue;
			}

			return nullptr;
		}

		inline uint32_t GetFamily(QueueOperation operation) const
		{
			for (const auto& [index, _, supportedOperations] : queues)
			{
				if ((uint32_t)(supportedOperations & operation))
					return index;
			}

			return InvalidHandle;
		}

		inline void Add(vk::Queue queue, QueueOperation operation, uint32_t index)
		{
			queues.push_back({ index, queue, operation });
		}

	public:
		std::vector<QueueData> queues;
	};
}