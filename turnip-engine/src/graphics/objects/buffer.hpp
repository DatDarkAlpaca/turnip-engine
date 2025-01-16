#pragma once
#include "common.hpp"

namespace tur
{
	using buffer_handle = handle_type;

	enum class BufferType
	{
		VERTEX_BUFFER,
		ELEMENT_ARRAY_BUFFER,
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