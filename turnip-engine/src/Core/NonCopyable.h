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

	class Singleton : public NonCopyableNorMoveable
	{
	public:
		static inline Singleton& Instance()
		{
			if (!s_Singleton)
				s_Singleton = MakeUnique<Singleton>();

			return *s_Singleton;
		}

	protected:
		static inline tur_unique<Singleton> s_Singleton = nullptr;
	};
}