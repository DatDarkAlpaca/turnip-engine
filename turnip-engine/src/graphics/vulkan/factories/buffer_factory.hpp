#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include "graphics/objects/buffer.hpp"
#include "graphics/vulkan/objects/buffer.hpp"

namespace tur::vulkan
{
	Buffer build_buffer(VmaAllocator allocator, const BufferDescriptor& descriptor, u32 size);
}