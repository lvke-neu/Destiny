#include "Engine.h"
#include "LogManager.h"
#include "RenderView.h"

namespace Destiny
{
	Engine::Engine()
	{
		m_logManager = std::make_shared<LogManager>();
		m_renderView = std::make_shared<RenderView>();
	}

	void Engine::initialize()
	{
		m_logManager->initialize();
		m_renderView->initialize();
	}

	void Engine::uninitialize()
	{
		m_logManager->uninitialize();
		m_renderView->uninitialize();
	}

	void Engine::run()
	{
		m_renderView->run();
	}
}