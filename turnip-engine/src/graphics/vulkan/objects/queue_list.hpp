#pragma once
#include <vulkan/vulkan.hpp>
#include <unordered_map>
#include <vector>

#include "utils/string_utils.hpp"
#include "queue_usage.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	using queue_index = handle_type;

	struct QueueList
	{
	public:
		void add_queue(const vk::Queue& queue, QueueUsage usage)
		{
			m_Queues.push_back(queue);
			m_Usage[static_cast<queue_index>(m_Queues.size() - 1)] = usage;
		}

		vk::Queue get(QueueUsage requestedUsage)
		{
			for (const auto& [index, usage] : m_Usage)
			{
				if (requestedUsage == usage)
					return m_Queues[index];
			}

			TUR_LOG_CRITICAL("Failed to locate requested queue: {}", static_cast<u16>(requestedUsage));
			return nullptr;
		}

	private:
		std::unordered_map<queue_index, QueueUsage> m_Usage;
		std::vector<vk::Queue> m_Queues;
	};
}