#pragma once
#include "Core/Memory/Memory.h"

namespace tur
{
	class NonCopyable
	{
	public:
		NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(NonCopyable&&) noexcept = delete;
	};

	class NonMoveable
	{
	public:
		NonMoveable() = default;
		NonMoveable(const NonMoveable&) = delete;
		NonMoveable& operator=(NonMoveable&&) noexcept = delete;
	};

	class NonCopyableNorMoveable
	{
	public:
		NonCopyableNorMoveable() = default;
		NonCopyableNorMoveable(const NonCopyableNorMoveable&) = delete;
		NonCopyableNorMoveable(NonCopyableNorMoveable&&) noexcept = delete;
		NonCopyableNorMoveable& operator=(const NonCopyableNorMoveable&) noexcept = delete;
		NonCopyableNorMoveable& operator=(NonCopyableNorMoveable&&) noexcept = delete;
	};
}