#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

namespace tur::vulkan
{
	struct Buffer
	{
	public:
		operator vk::Buffer() { return buffer; }

	public:
		void* data;
		uint32_t dataSize;

		vk::Buffer buffer;
		VmaAllocation allocation = {};
		VmaAllocationInfo info = {};
	};
}