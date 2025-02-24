#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include "graphics/objects/buffer.hpp"
#include "graphics/vulkan/objects/buffer.hpp"

namespace tur::vulkan
{
	inline Buffer create_buffer(VmaAllocator allocator, const BufferDescriptor& descriptor, u32 size)
	{
		vk::BufferCreateInfo createInfo = {};
		{
			createInfo.size = size;
			createInfo.usage = get_buffer_usage(descriptor.type);
		}
		VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
		VmaAllocationCreateInfo vmaAllocationCreateInfo = {};
		{
			vmaAllocationCreateInfo.usage = get_buffer_memory_usage(descriptor.memoryUsage);
			vmaAllocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
		}

		Buffer buffer = {};
		buffer.size = size;

		VkResult result = vmaCreateBuffer(
			allocator,
			reinterpret_cast<VkBufferCreateInfo*>(&createInfo),
			&vmaAllocationCreateInfo,
			reinterpret_cast<VkBuffer*>(&buffer.buffer),
			&buffer.allocation,
			&buffer.allocationInfo
		);

		if (result != VK_SUCCESS)
			TUR_LOG_CRITICAL("Failed to create buffer.");

		return buffer;
	}
}