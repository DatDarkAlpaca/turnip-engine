#include "pch.h"
#include "Logger.h"

#pragma warning(push, 0)
	#include <spdlog/sinks/stdout_color_sinks.h>
	#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

namespace tur
{
	void LoggerSystem::Initialize()
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("TurnipEngine.log", true));

		sinks[0]->set_pattern("[%T] [%n] [%^%l%$]: %v");
		sinks[1]->set_pattern("[%D | %T] [%n] [%l]: %v");

		// Core Logger:
		m_Logger = tur::MakeShared<spdlog::logger>("Turnip", std::begin(sinks), std::end(sinks));
		spdlog::register_logger(m_Logger);

		m_Logger->set_level(spdlog::level::trace);
		m_Logger->flush_on(spdlog::level::trace);

		// Info Logger:
		m_InfoLogger = tur::MakeShared<spdlog::logger>("Info", sinks[0]);
		spdlog::register_logger(m_InfoLogger);

		m_InfoLogger->set_level(spdlog::level::trace);
		m_InfoLogger->flush_on(spdlog::level::trace);
	}
}