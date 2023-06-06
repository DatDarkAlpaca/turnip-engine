#pragma once
#include <spdlog/spdlog.h>
#include "Common/Debug_Definition.h"

namespace tur
{
	void initialize_logger();
}

#ifdef TUR_DEBUG
	#define TUR_CORE_WARN(...)		::spdlog::warn(__VA_ARGS__)
	#define TUR_CORE_ERROR(...)		::spdlog::error(__VA_ARGS__)
	#define TUR_CORE_CRITICAL(...)	::spdlog::critical(__VA_ARGS__)
	#define TUR_CORE_DEBUG(...)		::spdlog::debug(__VA_ARGS__)
	#define TUR_CORE_TRACE(...)		::spdlog::trace(__VA_ARGS__)
	#define TUR_CORE_INFO(...)		::spdlog::info(__VA_ARGS__)
#elif defined(TUR_RELEASE)
	#define TUR_CORE_WARN(...)		::spdlog::warn(__VA_ARGS__)
	#define TUR_CORE_ERROR(...)		::spdlog::error(__VA_ARGS__)
	#define TUR_CORE_CRITICAL(...)	::spdlog::critical(__VA_ARGS__)
	#define TUR_CORE_DEBUG(...)
	#define TUR_CORE_TRACE(...)
	#define TUR_CORE_INFO(...)
#endif