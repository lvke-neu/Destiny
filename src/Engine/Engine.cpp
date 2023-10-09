#include "Engine.h"
#include "LogManager.h"

namespace Destiny
{
	Engine::Engine()
	{
		m_logManager = std::make_shared<LogManager>();
	}

	void Engine::initialize()
	{
		m_logManager->initialize();
	}

	void Engine::uninitialize()
	{
		m_logManager->uninitialize();
	}

	void Engine::run()
	{

	}
}