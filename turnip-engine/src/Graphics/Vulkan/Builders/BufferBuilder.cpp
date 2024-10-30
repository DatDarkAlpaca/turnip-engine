#include "pch.h"
#include "BufferBuilder.h"
#include "Graphics/Vulkan/Converters.h"

#include <vk_mem_alloc.h>

namespace tur::vulkan
{
	Buffer BufferBuilder::Build(VmaAllocator allocator, const BufferDescriptor& bufferDescriptor)
	{
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.pNext = nullptr;
		bufferCreateInfo.size = bufferDescriptor.dataSize;
		bufferCreateInfo.usage = static_cast<VkBufferUsageFlags>(GetBufferUsageFlags(bufferDescriptor.usageFlags));

		VmaAllocationCreateInfo vmaAllocCreateInfo = {};
		vmaAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		vmaAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

		Buffer buffer;
		buffer.data = bufferDescriptor.data;
		buffer.dataSize = bufferDescriptor.dataSize;

		VkResult result = vmaCreateBuffer(
			allocator,
			&bufferCreateInfo,
			&vmaAllocCreateInfo,
			reinterpret_cast<VkBuffer*>(&buffer.buffer),
			&buffer.allocation,
			&buffer.info
		);

		void* data = nullptr;
		vmaMapMemory(allocator, buffer.allocation, &data);
		memcpy(data, buffer.data, buffer.dataSize);
		vmaUnmapMemory(allocator, buffer.allocation);

		return buffer;
	}
}