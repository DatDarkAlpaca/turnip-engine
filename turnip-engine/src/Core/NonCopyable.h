#pragma once
#include "Core/Memory/Memory.h"

namespace tur
{
	template<typename T>
	class Singleton
	{
	private:
		Singleton& operator= (const Singleton&) = delete;
		Singleton& operator= (Singleton&&) = delete;

	protected:
		Singleton() = default;

	public:
		static T& Get() 
		{
			if (!s_Instance)
				s_Instance = new SingletonInstance;
			return *s_Instance;
		}

	private:
		struct SingletonInstance : public T
		{
			SingletonInstance()
				: T() 
			{
			}
		};

		static inline T* s_Instance = nullptr;
	};

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