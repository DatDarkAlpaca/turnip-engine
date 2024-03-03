#pragma once
#include "Common.h"
#include <vulkan/vulkan.hpp>
#include "Graphics/Vulkan/Objects/Objects.h"

namespace tur::vulkan
{
	class SemaphoreBuilder
	{
	public:
		vk::Semaphore Build(vk::Device& device)
		{
			vk::SemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.flags = vk::SemaphoreCreateFlags();

			try
			{
				return device.createSemaphore(semaphoreInfo);
			}
			catch (const vk::SystemError& err)
			{
				TUR_LOG_ERROR("Failed to create semaphore: {}", err.what());
				return nullptr;
			}
		}
	};

	class FenceBuilder
	{
	public:
		vk::Fence Build(vk::Device& device)
		{
			vk::FenceCreateInfo fenceInfo = {};
			fenceInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;

			try
			{
				return device.createFence(fenceInfo);
			}
			catch (const vk::SystemError& err)
			{
				TUR_LOG_ERROR("Failed to create fence: {}", err.what());
				return nullptr;
			}
		}
	};
}