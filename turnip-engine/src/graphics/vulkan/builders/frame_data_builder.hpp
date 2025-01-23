#pragma once
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/objects/vulkan_state.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	void initialize_frame_data(VulkanState& state)
	{
		// Command Pool & Buffers:
		vk::CommandPoolCreateInfo poolCreateInfo = {};
		{
			poolCreateInfo.flags = vk::CommandPoolCreateFlags();
			poolCreateInfo.queueFamilyIndex = state.queueList.get_family_index(QueueUsage::TRANSFER);
		}

		for (auto& frame : state.frameDataHolder.get_frames())
		{
			try
			{
				frame.commandPool = state.logicalDevice.createCommandPool(poolCreateInfo);
			}
			catch (vk::SystemError& err)
			{
				TUR_LOG_CRITICAL("Failed to create frame command pools. {}", err.what());
			}

			vk::CommandBufferAllocateInfo allocateInfo = {};
			{
				allocateInfo.commandPool = frame.commandPool;
				allocateInfo.commandBufferCount = 1;
				allocateInfo.level = vk::CommandBufferLevel::ePrimary;
			}

			frame.commandBuffer = state.logicalDevice.allocateCommandBuffers(allocateInfo).front();
		}

		// Synchonization primitives:
		for (auto& frame : state.frameDataHolder.get_frames())
		{
			vk::FenceCreateInfo createInfo = {};
			createInfo.flags = vk::FenceCreateFlagBits::eSignaled;

			frame.recordingFence = state.logicalDevice.createFence(createInfo);
		}
	}
}