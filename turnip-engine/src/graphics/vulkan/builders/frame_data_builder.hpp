#pragma once
#include <vulkan/vulkan.hpp>

#include "graphics/vulkan/objects/vulkan_state.hpp"
#include "common.hpp"

namespace tur::vulkan
{
	void initialize_frame_data(VulkanState& state)
	{
		// Command buffers:
		for (auto& frame : state.frameDataHolder.get_frames())
		{
			vk::CommandBufferAllocateInfo allocateInfo = {};
			{
				allocateInfo.commandPool = state.commandPool;
				allocateInfo.commandBufferCount = 1;
				allocateInfo.level = vk::CommandBufferLevel::ePrimary;
			}

			try
			{
				frame.commandBuffer = state.logicalDevice.allocateCommandBuffers(allocateInfo).front();
			}
			catch (vk::SystemError err)
			{
				TUR_LOG_CRITICAL("Failed to allocate frame command buffer");
			}
		}

		// Synchonization primitives:
		for (auto& frame : state.frameDataHolder.get_frames())
		{
			vk::FenceCreateInfo createInfo = {};
			createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
			frame.recordingFence = state.logicalDevice.createFence(createInfo);
		}

		for (auto& frame : state.frameDataHolder.get_frames())
		{
			vk::SemaphoreCreateInfo createInfo = {};
			frame.imageAvailableSemaphore = state.logicalDevice.createSemaphore(createInfo);
			frame.renderFinishedSemaphore = state.logicalDevice.createSemaphore(createInfo);
		}
	}
}