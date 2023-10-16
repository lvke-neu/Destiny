#include "Engine.h"
#include "LogManager.h"
#include "EventSystem.h"
#include "Timer.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	Engine::Engine()
	{
		m_logManager = std::make_shared<LogManager>();
		m_eventSystem = std::make_shared<EventSystem>();
		m_graphicsSystem = std::make_shared<GraphicsSystem>();
		m_timer = std::make_shared<Timer>();
	}

	void Engine::initialize(long long hwnd)
	{
		m_logManager->initialize();
		m_eventSystem->initialize();
		m_graphicsSystem->initialize(hwnd);
		m_timer->reset();
	}

	void Engine::uninitialize()
	{
		m_logManager->uninitialize();
		m_eventSystem->uninitialize();
		m_graphicsSystem->uninitialize();
	}

	void Engine::update()
	{
		m_timer->tick();
		m_eventSystem->dispatchEvent(EventType::Tick, m_timer->deltaTime());
		m_graphicsSystem->draw();
	}
}