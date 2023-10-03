#pragma once
#include <spdlog/spdlog.h>
#include <memory>

namespace Destiny
{
	class LogManager
	{
	public:
		~LogManager();
		void initialize();
		std::shared_ptr<spdlog::logger> getCoreLogger();
	private:
		std::shared_ptr<spdlog::logger> m_coreLogger;
	};

	inline std::shared_ptr<spdlog::logger> LogManager::getCoreLogger()
	{
		return m_coreLogger;
	}
}