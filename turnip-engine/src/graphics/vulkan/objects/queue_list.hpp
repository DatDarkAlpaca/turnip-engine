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
	using queue_family_index = handle_type;

	struct QueueList
	{
	public:
		void add_queue(const vk::Queue& queue, queue_family_index familyIndex, QueueUsage usage)
		{
			m_Queues.push_back({ queue, familyIndex });
			m_Usage[static_cast<queue_index>(m_Queues.size() - 1)] = usage;
		}

		vk::Queue get(QueueUsage requestedUsage)
		{
			for (const auto& [index, usage] : m_Usage)
			{
				if (requestedUsage & usage)
					return m_Queues[index].first;
			}

			TUR_LOG_CRITICAL("Failed to locate requested queue: {}", static_cast<u16>(requestedUsage));
			return nullptr;
		}

		queue_family_index get_family_index(QueueUsage requestedUsage)
		{
			for (const auto& [index, usage] : m_Usage)
			{
				if((usage & QueueUsage::GRAPHICS) && (requestedUsage == QueueUsage::TRANSFER))
					return m_Queues[index].second;

				if ((usage & QueueUsage::COMPUTE) && (requestedUsage == QueueUsage::TRANSFER))
					return m_Queues[index].second;

				if (requestedUsage & usage)
					return m_Queues[index].second;
			}

			TUR_LOG_CRITICAL("Failed to locate requested queue: {}", static_cast<u16>(requestedUsage));
			return invalid_handle;
		}

	private:
		std::unordered_map<queue_index, QueueUsage> m_Usage;
		std::vector<std::pair<vk::Queue, queue_family_index>> m_Queues;
	};
}