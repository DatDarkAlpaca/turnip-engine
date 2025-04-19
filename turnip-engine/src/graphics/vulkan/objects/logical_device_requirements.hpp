#pragma once
#include "queue_list.hpp"

namespace tur::vulkan
{	
	struct LogicalDeviceRequirements
	{
		std::vector<QueueUsage> requiredQueueOperations = { QueueUsage::GRAPHICS, QueueUsage::PRESENT };
	};
}