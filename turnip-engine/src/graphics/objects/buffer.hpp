#pragma once
#include "common.hpp"

namespace tur
{
	enum class BufferIndexType : uint8_t
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
		VERTEX_BUFFER,
		INDEX_BUFFER,
		UNIFORM_BUFFER,
	};

	enum class BufferUsage
	{
		STATIC,
		DYNAMIC,
		STREAM
	};

	struct BufferDescriptor
	{
		BufferType type;
		BufferUsage usage;
		bool hostVisible;
		bool hostCoherent;
	};
}