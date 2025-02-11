#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/objects/physical_device_requirements.hpp"
#include "graphics/vulkan/objects/vulkan_state.hpp"
#include "graphics/vulkan/vulkan_constants.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	void initialize_physical_device(VulkanState& state, const PhysicalDeviceRequirements& requirements);
}