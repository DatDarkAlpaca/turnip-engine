#pragma once
#include "common.hpp"

namespace tur
{
	enum class BufferIndexType : u8
	{
		UNSIGNED_BYTE,
		UNSIGNED_SHORT,
		UNSIGNED_INT
	};
}

namespace tur
{
	using buffer_handle = handle_type;

	enum class BufferType
	{
		NONE				= 0,
		VERTEX_BUFFER		= 1 << 0,
		INDEX_BUFFER		= 1 << 1,
		UNIFORM_BUFFER		= 1 << 2,
		TRANSFER_SRC		= 1 << 3,
		TRANSFER_DST		= 1 << 4,
	};

	inline BufferType operator| (BufferType lhs, BufferType rhs)
	{
		return static_cast<BufferType>(static_cast<u32>(lhs) | static_cast<u32>(rhs));
	}

	enum class BufferUsage
	{
		STATIC,
		DYNAMIC,
		STREAM
	};

	enum class BufferMemoryUsage
	{
		CPU_ONLY,
		GPU_ONLY,
		GPU_TO_CPU,
		CPU_TO_GPU
	};

	struct BufferDescriptor
	{
		BufferType type;
		BufferUsage usage;
		BufferMemoryUsage memoryUsage;
	};
}