#pragma once
#include <memory>

// Raw pointer purpose identifier
#define NON_OWNING

namespace tur
{
	template<typename T, typename Deleter = std::default_delete<T>>
	using tur_unique = std::unique_ptr<T, Deleter>;

	template<typename T, typename ... Args>
	constexpr tur_unique<T> make_unique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using tur_shared = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr tur_shared<T> make_shared(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using tur_weak = std::weak_ptr<T>;
}