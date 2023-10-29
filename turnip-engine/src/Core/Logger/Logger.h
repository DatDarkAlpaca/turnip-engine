#pragma once
#include "Core/Defines.h"
#include <spdlog/spdlog.h>

namespace tur
{
	inline void InitializeLogger()
	{
#ifdef TUR_DEBUG
		spdlog::set_level(spdlog::level::debug);
#endif
	}
}

#ifdef TUR_DEBUG
	#define TUR_LOG_CRITICAL(...)	{ ::spdlog::critical(__VA_ARGS__); TUR_BREAKPOINT(); }
	#define TUR_LOG_ERROR(...)		::spdlog::error(__VA_ARGS__)
	#define TUR_LOG_WARN(...)		::spdlog::warn(__VA_ARGS__)
	#define TUR_LOG_TRACE(...)		::spdlog::trace(__VA_ARGS__)
	#define TUR_LOG_DEBUG(...)		::spdlog::debug(__VA_ARGS__)
	#define TUR_LOG_INFO(...)		::spdlog::info(__VA_ARGS__)

#else
	#define TUR_LOG_CRITICAL(...)	::spdlog::critical(__VA_ARGS__);
	#define TUR_LOG_ERROR(...)		::spdlog::error(__VA_ARGS__)
	#define TUR_LOG_WARN(...)
	#define TUR_LOG_DEBUG(...)	
	#define TUR_LOG_TRACE(...)
	#define TUR_LOG_INFO(...)
#endif