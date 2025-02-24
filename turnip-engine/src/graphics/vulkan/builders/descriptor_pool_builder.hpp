#pragma once
#pragma once
#pragma once
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/objects/vulkan_state.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	void initialize_descriptor_pool(VulkanState& state)
	{
		state.frameDataHolder.get_frames();

		vk::CommandPoolCreateInfo poolCreateInfo = {};
		{
			poolCreateInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
			poolCreateInfo.queueFamilyIndex = state.queueList.get_family_index(QueueUsage::TRANSFER);
		}

		try
		{
			state.commandPool = state.logicalDevice.createCommandPool(poolCreateInfo);
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_CRITICAL("Failed to create frame command pool. {}", err.what());
		}
	}
}