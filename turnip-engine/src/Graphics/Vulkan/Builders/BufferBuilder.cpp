#include "pch.h"
#include "BufferBuilder.h"

#include <vk_mem_alloc.h>
#define VMA_IMPLEMENTATION

namespace tur::vulkan
{
	Buffer BufferBuilder::Build(VmaAllocator allocator, const BufferDescriptor& bufferDescriptor)
	{
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.pNext = nullptr;
		bufferCreateInfo.size = bufferDescriptor.dataSize;
		bufferCreateInfo.usage = bufferDescriptor.usageFlag;

		VmaAllocationCreateInfo vmaAllocCreateInfo = {};
		//vmaAllocCreateInfo.usage = bufferDescriptor.memoryUsage;
		vmaAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

		Buffer buffer;
		VkResult resul = vmaCreateBuffer(
			allocator,
			&bufferCreateInfo,
			&vmaAllocCreateInfo,
			reinterpret_cast<VkBuffer*>(&buffer.buffer),
			&buffer.allocation,
			&buffer.info
		);

		return buffer;
	}
}