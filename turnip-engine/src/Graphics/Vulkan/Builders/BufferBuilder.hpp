#pragma once
#include <vk_mem_alloc.h>

#include "Rendering/Resource/Buffer.h"
#include "Graphics/Vulkan/Objects/Buffer.h"

namespace tur::vulkan
{
	class BufferBuilder
	{
	public:
		Buffer Build(VmaAllocator allocator, const BufferDescriptor& bufferDescriptor);
	};
}