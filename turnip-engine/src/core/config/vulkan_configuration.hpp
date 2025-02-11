#pragma once
#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <string>

#include "graphics/vulkan/objects/instance_requirements.hpp"
#include "graphics/vulkan/objects/physical_device_requirements.hpp"
#include "graphics/vulkan/objects/logical_device_requirements.hpp"

namespace tur::vulkan
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(InstanceRequirements,
		layers,
		extensions,
		enableDrawing,
		enableValidationLayers
	);

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PhysicalDeviceRequirements,
		extensions,
		preferDiscrete
	);

	// Logical Device:
	inline void to_json(nlohmann::json& json, const LogicalDeviceRequirements& requirements)
	{
		json["requiredQueueOperations"] = nlohmann::json::array();

		for (const auto& operation : requirements.requiredQueueOperations)
		{
			for(const auto& usageString : get_queue_usage_strings(operation))
				json["requiredQueueOperations"].push_back(usageString);
		}
	}

	inline void from_json(const nlohmann::json& json, LogicalDeviceRequirements& requirements)
	{
		for (const auto& operation : json["requiredQueueOperations"])
			requirements.requiredQueueOperations.push_back(get_queue_usage_from_string(operation));
	}

	struct VulkanConfiguration
	{
		InstanceRequirements instanceRequirements;
		PhysicalDeviceRequirements physicalDeviceRequirements;
		LogicalDeviceRequirements logicalDeviceRequirements;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(VulkanConfiguration, instanceRequirements, physicalDeviceRequirements, logicalDeviceRequirements);
	};
}