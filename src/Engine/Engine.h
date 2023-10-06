#pragma once
#include "Utility.h"
#include <memory>

namespace Destiny
{
	class LogManager;
	class RenderView;
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
		void run();
	public:
		std::shared_ptr<LogManager> getLogManager();
		std::shared_ptr<RenderView> getRenderView();
	private:
		std::shared_ptr<LogManager> m_logManager;
		std::shared_ptr<RenderView> m_renderView;
	};

	inline std::shared_ptr<LogManager> Engine::getLogManager()
	{
		return m_logManager;
	}

	inline std::shared_ptr<RenderView> Engine::getRenderView()
	{
		return m_renderView;
	}
}