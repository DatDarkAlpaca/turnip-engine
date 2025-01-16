#pragma once
#include "core/defines.hpp"
#include "core/memory/memory.hpp"

#pragma warning(push, 0)
	#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace tur
{
	inline tur_shared<spdlog::logger> MainLogger;
	inline tur_shared<spdlog::logger> InfoLogger;

	void initialize_logger_system();
}

#ifdef TUR_DEBUG
	#define TUR_LOG_CRITICAL(...)	{ ::tur::MainLogger->critical(__VA_ARGS__); TUR_BREAKPOINT(); }
	#define TUR_LOG_ERROR(...)		  ::tur::MainLogger->error(__VA_ARGS__)
	#define TUR_LOG_WARN(...)		  ::tur::MainLogger->warn(__VA_ARGS__)
	#define TUR_LOG_DEBUG(...)		  ::tur::MainLogger->debug(__VA_ARGS__)
	#define TUR_LOG_TRACE(...)		  ::tur::InfoLogger->trace(__VA_ARGS__)
	#define TUR_LOG_INFO(...)		  ::tur::InfoLogger->info(__VA_ARGS__)

#else
	#define TUR_LOG_CRITICAL(...)	{ ::tur::MainLogger->critical(__VA_ARGS__); TUR_BREAKPOINT(); }
	#define TUR_LOG_ERROR(...)		  ::tur::MainLogger->error(__VA_ARGS__)
	#define TUR_LOG_WARN(...)		{ }
	#define TUR_LOG_DEBUG(...)		{ }
	#define TUR_LOG_TRACE(...)		{ }
	#define TUR_LOG_INFO(...)		{ }
#endif