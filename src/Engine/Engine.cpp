#include "Engine.h"
#include "LogManager.h"

namespace Destiny
{
	Engine::Engine()
	{
		m_logManager = std::shared_ptr<LogManager>(new LogManager);
	}

	void Engine::initialize()
	{
		m_logManager->initialize();
	}

	void Engine::uninitialize()
	{
	
	}

	void Engine::run()
	{

	}
}