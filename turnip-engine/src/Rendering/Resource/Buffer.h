#pragma once
#include "Common.h"
#include <numeric>

namespace tur
{
	enum class UsageFlag : uint32_t
	{
		NONE					= 0 << 0,
		ARRAY_BUFFER			= 1 << 0,
		INDEX_BUFFER			= 1 << 1,
		UNIFORM_BUFFER			= 1 << 2,
		
		TRANSFER_DST			= 1 << 3,
		TRANSFER_SRC			= 1 << 4,

		MAX_ITEMS				= 5
	};
	DECLARE_STATE(tur::UsageFlag);

	enum class DataStorage : uint32_t
	{
		STREAM_DRAW,
		STREAM_READ,
		STREAM_COPY,

		STATIC_DRAW,
		STATIC_READ,
		STATIC_COPY,

		DYNAMIC_DRAW,
		DYNAMIC_READ,
		DYNAMIC_COPY,
	};

	struct BufferDescriptor
	{
		// TODO: Use an explicit standard for usage flags across apis.
		State<UsageFlag> usageFlags = {};
		
		void* data;
		uint32_t dataSize;

		/* Optional: Only used in OpenGL.
		*/
		DataStorage dataStorage = DataStorage::STATIC_DRAW;
	};

	enum class BufferHandle : uint32_t { INVALID = InvalidHandle };
}