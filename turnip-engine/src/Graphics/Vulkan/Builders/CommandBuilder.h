#pragma once
#include <optional>
#include <vulkan/vulkan.hpp>
#include "Graphics/Vulkan/Objects/Objects.h"

namespace tur::vulkan
{
	class CommandPoolBuilder
	{
	public:
		CommandPoolBuilder& SetArguments(vk::Device& device, const vulkan::QueueCluster& queues)
		{
			m_Device = device;
			m_Queues = queues;

			return *this;
		}

	public:
		std::optional<vk::CommandPool> Build()
		{
			vk::CommandPoolCreateInfo poolInfo;
			poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
			poolInfo.queueFamilyIndex = m_Queues.GetFamily(vulkan::QueueOperation::GRAPHICS);

			try
			{
				return m_Device.createCommandPool(poolInfo);
			}
			catch (vk::SystemError err)
			{
				TUR_BREAKPOINT(); // TODO
				return nullptr;
			}
		}

	private:
		vk::Device m_Device;
		vulkan::QueueCluster m_Queues;
	};

	class CommandBufferBuilder
	{
	public:
		CommandBufferBuilder& SetArguments(vk::Device& device, const vk::CommandPool& commandPool)
		{
			m_Device = device;
			m_CommandPool = commandPool;

			return *this;
		}

	public:
		std::optional<vk::CommandBuffer> Build()
		{
			vk::CommandBufferAllocateInfo allocInfo = {};
			allocInfo.commandPool = m_CommandPool;
			allocInfo.level = vk::CommandBufferLevel::ePrimary;
			allocInfo.commandBufferCount = 1;

			try 
			{
				vk::CommandBuffer commandBuffer = m_Device.allocateCommandBuffers(allocInfo)[0];
				return commandBuffer;
			}
			catch (vk::SystemError err) 
			{
				TUR_LOG_ERROR("Failed to allocate command buffer: {}", err.what());
				return nullptr;
			}
		}

	private:
		vk::Device m_Device;
		vk::CommandPool m_CommandPool;
	};
}