#include "Engine.h"
#include "LogManager.h"
#include "RenderView.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	Engine::Engine()
	{
		m_logManager = std::make_shared<LogManager>();
		m_graphicsSystem = std::make_shared<GraphicsSystem>();
	}

	void Engine::initialize()
	{
		m_logManager->initialize();
		m_graphicsSystem->initialize();
	}

	void Engine::uninitialize()
	{
		m_logManager->uninitialize();
		m_graphicsSystem->uninitialize();
	}

	void Engine::update()
	{

	}
}