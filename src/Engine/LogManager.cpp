#include "LogManager.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


namespace Destiny
{
	LogManager::LogManager() : m_coreLogger(nullptr)
	{

	}

	void LogManager::initialize()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Enternity.log", true));

		logSinks[0]->set_pattern("[%T]%v%$");
		logSinks[1]->set_pattern("[%T]%v%$");

		m_coreLogger = std::make_shared<spdlog::logger>("Enternity", begin(logSinks), end(logSinks));
		spdlog::register_logger(m_coreLogger);
		m_coreLogger->set_level(spdlog::level::trace);
		m_coreLogger->flush_on(spdlog::level::trace);
	}

	void LogManager::uninitialize()
	{

	}
}