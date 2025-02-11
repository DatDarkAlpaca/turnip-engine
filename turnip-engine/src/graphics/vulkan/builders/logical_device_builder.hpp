#pragma once
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/objects/vulkan_state.hpp"
#include "core/config/config_data.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	void initialize_logical_device(VulkanState& state, const VulkanConfiguration& vulkanConfig);
}