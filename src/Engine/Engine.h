#pragma once
#include "Utility.h"
#include <memory>

namespace Destiny
{
	struct EngineSetting
	{
		long long Hwnd;
		int DataLoadingThreadCount;
	};

	class Timer;
	class LogManager;
	class EventSystem;
	class ThreadPool;
	class GraphicsSystem;
	class Engine
	{
		SINGLETON(Engine);
	private:
		Engine();
		~Engine() = default;
		Engine(const Engine&) = default;
		Engine& operator=(const Engine&) = default;
	public:
		void initialize(const EngineSetting& setting);
		void uninitialize();
		void update();
	public:
		std::shared_ptr<LogManager> getLogManager();
		std::shared_ptr<EventSystem> getEventSystem();
		std::shared_ptr<ThreadPool> getThreadPool();
		std::shared_ptr<GraphicsSystem> getGraphicsSystem();
	private:
		std::shared_ptr<Timer> m_timer;
		std::shared_ptr<LogManager> m_logManager;
		std::shared_ptr<EventSystem> m_eventSystem;
		std::shared_ptr<ThreadPool> m_dataLoadThreadPool;
		std::shared_ptr<GraphicsSystem> m_graphicsSystem;
	};

	inline std::shared_ptr<LogManager> Engine::getLogManager()
	{
		return m_logManager;
	}

	inline std::shared_ptr<EventSystem> Engine::getEventSystem()
	{
		return m_eventSystem;
	}

	inline std::shared_ptr<ThreadPool> Engine::getThreadPool()
	{
		return m_dataLoadThreadPool;
	}

	inline std::shared_ptr<Destiny::GraphicsSystem> Engine::getGraphicsSystem()
	{
		return m_graphicsSystem;
	}
}