#pragma once
#include "Core/Defines.hpp"
#include "Core/NonCopyable.hpp"
#include "Core/Memory/Memory.hpp"

#pragma warning(push, 0)
	#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace tur::logger
{
	inline tur_shared<spdlog::logger> MainLogger;
	inline tur_shared<spdlog::logger> InfoLogger;

	void InitializeLogger();
}

#ifdef TUR_DEBUG
	#define TUR_LOG_CRITICAL(...)	{ ::tur::logger::MainLogger->critical(__VA_ARGS__); TUR_BREAKPOINT(); }
	#define TUR_LOG_ERROR(...)		  ::tur::logger::MainLogger->error(__VA_ARGS__)
	#define TUR_LOG_WARN(...)		  ::tur::logger::MainLogger->warn(__VA_ARGS__)
	#define TUR_LOG_DEBUG(...)		  ::tur::logger::MainLogger->debug(__VA_ARGS__)
	#define TUR_LOG_TRACE(...)		  ::tur::logger::InfoLogger->trace(__VA_ARGS__)
	#define TUR_LOG_INFO(...)		  ::tur::logger::InfoLogger->info(__VA_ARGS__)

#else
	#define TUR_LOG_CRITICAL(...)	{ ::tur::logger::MainLogger->critical(__VA_ARGS__); TUR_BREAKPOINT(); }
	#define TUR_LOG_ERROR(...)		  ::tur::logger::MainLogger->error(__VA_ARGS__)
	#define TUR_LOG_WARN(...)		{ }
	#define TUR_LOG_DEBUG(...)		{ }
	#define TUR_LOG_TRACE(...)		{ }
	#define TUR_LOG_INFO(...)		{ }
#endif