#pragma once
#include "Utility.h"
#include <memory>

namespace Destiny
{
	class LogManager;
	class EventSystem;
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
		void initialize(long long hwnd);
		void uninitialize();
		void update();
	public:
		std::shared_ptr<LogManager> getLogManager();
		std::shared_ptr<EventSystem> getEventSystem();
		std::shared_ptr<GraphicsSystem> getGraphicsSystem();
	private:
		std::shared_ptr<LogManager> m_logManager;
		std::shared_ptr<EventSystem> m_eventSystem;
		std::shared_ptr<GraphicsSystem> m_graphicsSystem;
	};

	inline std::shared_ptr<LogManager> Engine::getLogManager()
	{
		return m_logManager;
	}

	inline std::shared_ptr<Destiny::GraphicsSystem> Engine::getGraphicsSystem()
	{
		return m_graphicsSystem;
	}

	inline std::shared_ptr<EventSystem> Engine::getEventSystem()
	{
		return m_eventSystem;
	}
}