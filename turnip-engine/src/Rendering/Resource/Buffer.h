#pragma once
#include "Common.h"
#include <numeric>

namespace tur
{
	enum class BindingFlag : uint32_t
	{
		ARRAY_BUFFER,
		ATOMIC_COUNTER_BUFFER,
		COPY_READ_BUFFER,
		COPY_WRITE_BUFFER,
		DISPATCH_INDIRECT_BUFFER,
		DRAW_INDIRECT_BUFFER,
		ELEMENT_ARRAY_BUFFER,
		PIXEL_PACK_BUFFER,
		PIXEL_UNPACK_BUFFER,
		QUERY_BUFFER,
		SHADER_STORAGE_BUFFER,
		TEXTURE_BUFFER,
		TRANSFORM_FEEDBACK_BUFFER,
		UNIFORM_BUFFER
	};

	enum class UsageFlag : uint32_t
	{
		STREAM_DRAW, 
		STREAM_READ, 
		STREAM_COPY, 
		STATIC_DRAW, 
		STATIC_READ, 
		STATIC_COPY,
		DYNAMIC_DRAW,
		DYNAMIC_READ, 
		DYNAMIC_COPY
	};

	struct BufferDescriptor
	{
		BindingFlag bindingFlag;
		UsageFlag usageFlag;
		void* data;
		uint32_t dataSize;
	};

	enum class BufferHandle : uint32_t { INVALID = InvalidHandle };
}