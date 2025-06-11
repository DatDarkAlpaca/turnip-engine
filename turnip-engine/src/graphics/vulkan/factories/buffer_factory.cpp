#include "pch.hpp"
#include "buffer_factory.hpp"

namespace tur::vulkan
{
	Buffer build_buffer(VmaAllocator allocator, const BufferDescriptor& descriptor, u32 size)
	{
		vk::BufferCreateInfo createInfo = {};
		{
			createInfo.size = size;
			createInfo.usage = get_buffer_usage(descriptor.type);
		}

		// TODO: add buffer flags.
		VmaAllocationCreateInfo vmaAllocationCreateInfo = {};
		{
			vmaAllocationCreateInfo.usage = get_buffer_memory_usage(descriptor.memoryUsage);
			vmaAllocationCreateInfo.flags = get_buffer_memory_flags(descriptor.usage);
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