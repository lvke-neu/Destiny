#pragma once
#include "Utility.h"
#include <memory>

namespace Destiny
{
	class LogManager;
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
		void initialize(long long hwnd, unsigned int width, unsigned int height);
		void uninitialize();
		void update();
	public:
		std::shared_ptr<LogManager> getLogManager();
		std::shared_ptr<GraphicsSystem> getGraphicsSystem();
	private:
		std::shared_ptr<LogManager> m_logManager;
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
}