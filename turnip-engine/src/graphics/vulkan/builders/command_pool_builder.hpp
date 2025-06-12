#pragma once
#pragma once
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/objects/vulkan_state.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	void initialize_command_pool(VulkanState& state);
}