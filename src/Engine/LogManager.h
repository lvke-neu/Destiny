#pragma once
#include "ParallelSystem.h"
#include <spdlog/spdlog.h>
#include <memory>

namespace Destiny
{
	class LogManager : public ParallelSystem
	{
	public:
		virtual void initialize() override;
		virtual void uninitialize() override {}
		virtual void tick() override {}
		std::shared_ptr<spdlog::logger> getCoreLogger();
	private:
		std::shared_ptr<spdlog::logger> m_coreLogger;
	};

	inline std::shared_ptr<spdlog::logger> LogManager::getCoreLogger()
	{
		return m_coreLogger;
	}
}