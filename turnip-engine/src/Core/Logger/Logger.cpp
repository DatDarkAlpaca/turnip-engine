#include "pch.hpp"
#include "Logger.hpp"

#pragma warning(push, 0)
	#include <spdlog/sinks/stdout_color_sinks.h>
	#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

namespace tur::logger
{
	void InitializeLogger()
	{
		std::vector<spdlog::sink_ptr> sinks;

		std::string logFilename = TUR_PROJECT_NAME;
		logFilename.append(".log");

		sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilename, true));

		sinks[0]->set_pattern("[%T] [%n] [%^%l%$]: %v");
		sinks[1]->set_pattern("[%D | %T] [%n] [%l]: %v");

		// Core Logger:
		MainLogger = tur::MakeShared<spdlog::logger>(TUR_PROJECT_NAME, std::begin(sinks), std::end(sinks));
		spdlog::register_logger(MainLogger);

		MainLogger->set_level(spdlog::level::trace);
		MainLogger->flush_on(spdlog::level::trace);

		// Info Logger:
		InfoLogger = tur::MakeShared<spdlog::logger>("Info", sinks[0]);
		spdlog::register_logger(InfoLogger);

		InfoLogger->set_level(spdlog::level::trace);
		InfoLogger->flush_on(spdlog::level::trace);
	}
}