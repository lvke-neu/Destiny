#include "Engine.h"
#include "LogManager.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	Engine::Engine()
	{
		m_logManager = std::make_shared<LogManager>();
		m_graphicsSystem = std::make_shared<GraphicsSystem>();
	}

	void Engine::initialize(long long hwnd, unsigned int width, unsigned int height)
	{
		m_logManager->initialize();
		m_graphicsSystem->initialize(hwnd, width, height);
	}

	void Engine::uninitialize()
	{
		m_logManager->uninitialize();
		m_graphicsSystem->uninitialize();
	}

	void Engine::update()
	{
		m_graphicsSystem->draw();
	}
}