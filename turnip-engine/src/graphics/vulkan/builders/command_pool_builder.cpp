#include "pch.hpp"
#include "command_pool_builder.hpp"

namespace tur::vulkan
{
	void initialize_command_pool(VulkanState& state)
	{
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