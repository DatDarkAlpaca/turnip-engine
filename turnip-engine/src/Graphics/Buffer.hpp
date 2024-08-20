#pragma once
#include "CommonGraphics.hpp"

namespace tur
{
	using buffer_handle = handle_type;

	enum class BufferUsage
	{
		VERTEX_BUFFER,
		INDEX_BUFFER,
		UNIFORM_BUFFER,
		STORAGE_BUFFER,
		SHADER_STORAGE_BUFFER,
		INDIRECT_BUFFER,
		TRANSFER_SRC,
		TRANSFER_DST
	};

	enum class MemoryUsage 
	{
		GPU_EXCLUSIVE,
		CPU_EXCLUSIVE,
		CPU_TO_GPU,
		GPU_TO_CPU,
	};

	struct BufferDescriptor
	{
		size_t size;
		BufferUsage usage;
		MemoryUsage memoryUsage;
	};
}