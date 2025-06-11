#pragma once
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/objects/vulkan_state.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	void initialize_frame_data(VulkanState& state);
}