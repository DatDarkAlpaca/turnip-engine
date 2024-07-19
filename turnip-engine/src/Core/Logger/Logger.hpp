#pragma once
#include "Core/Defines.hpp"
#include "Core/NonCopyable.hpp"

#pragma warning(push, 0)
	#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace tur
{
	class LoggerSystem : public Singleton<LoggerSystem>
	{
	public:
		void Initialize();

	public:
		inline tur_shared<spdlog::logger>& GetLogger() { return m_Logger; }
		inline tur_shared<spdlog::logger>& GetInfoLogger() { return m_InfoLogger; }

	private:
		tur_shared<spdlog::logger> m_Logger;
		tur_shared<spdlog::logger> m_InfoLogger;
	};
}

#ifdef TUR_DEBUG
	#define TUR_LOG_CRITICAL(...)	{ ::tur::LoggerSystem::Get().GetLogger()->critical(__VA_ARGS__); TUR_BREAKPOINT(); }
	#define TUR_LOG_ERROR(...)		::tur::LoggerSystem::Get().GetLogger()->error(__VA_ARGS__)
	#define TUR_LOG_WARN(...)		::tur::LoggerSystem::Get().GetLogger()->warn(__VA_ARGS__)
	#define TUR_LOG_TRACE(...)		::tur::LoggerSystem::Get().GetInfoLogger()->trace(__VA_ARGS__)
	#define TUR_LOG_DEBUG(...)		::tur::LoggerSystem::Get().GetLogger()->debug(__VA_ARGS__)
	#define TUR_LOG_INFO(...)		::tur::LoggerSystem::Get().GetInfoLogger()->info(__VA_ARGS__)

#else
	#define TUR_LOG_CRITICAL(...)	{ ::tur::LoggerSystem::Get().GetLogger()->critical(__VA_ARGS__); TUR_BREAKPOINT(); }
	#define TUR_LOG_ERROR(...)		::tur::LoggerSystem::Get().GetLogger()->error(__VA_ARGS__)
	#define TUR_LOG_WARN(...)		{ }
	#define TUR_LOG_DEBUG(...)		{ }
	#define TUR_LOG_TRACE(...)		{ }
	#define TUR_LOG_INFO(...)		{ }
#endif