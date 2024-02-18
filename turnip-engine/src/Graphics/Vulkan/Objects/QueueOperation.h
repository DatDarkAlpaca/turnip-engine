#pragma once
#include <numeric>

namespace tur::vulkan
{
	enum class QueueOperation : uint32_t
	{
		NONE			= 0,
		GRAPHICS		= 1 << 0,
		PRESENT			= 1 << 1,
		COMPUTE			= 1 << 2,
		TRANSFER		= 1 << 3,
		SPARSE_BINDING  = 1 << 4
	};

	inline QueueOperation operator| (QueueOperation lhs, QueueOperation rhs)
	{
		return static_cast<QueueOperation>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}

	inline QueueOperation operator& (QueueOperation lhs, QueueOperation rhs)
	{
		return static_cast<QueueOperation>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}

	inline QueueOperation& operator|= (QueueOperation& lhs, QueueOperation rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}
}