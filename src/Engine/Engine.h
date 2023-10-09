#pragma once
#include "Utility.h"
#include <memory>

namespace Destiny
{
	class LogManager;
	class Engine
	{
		SINGLETON(Engine);
	private:
		Engine();
		~Engine() = default;
		Engine(const Engine&) = default;
		Engine& operator=(const Engine&) = default;
	public:
		void initialize();
		void uninitialize();
		void update();
	public:
		std::shared_ptr<LogManager> getLogManager();
	private:
		std::shared_ptr<LogManager> m_logManager;
	};

	inline std::shared_ptr<LogManager> Engine::getLogManager()
	{
		return m_logManager;
	}
}