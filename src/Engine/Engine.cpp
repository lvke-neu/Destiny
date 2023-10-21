#include "Engine.h"
#include "LogManager.h"
#include "EventSystem.h"
#include "ThreadPool.h"
#include "Timer.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	Engine::Engine()
	{
		m_timer = std::make_shared<Timer>();
		m_logManager = std::make_shared<LogManager>();
		m_eventSystem = std::make_shared<EventSystem>();
		m_threadPool = std::make_shared<ThreadPool>();
		m_graphicsSystem = std::make_shared<GraphicsSystem>();
	}

	void Engine::initialize(const EngineSetting& setting)
	{
		m_logManager->initialize();
		m_eventSystem->initialize();
		m_threadPool->initialize(setting.DataLoadingThreadCount);
		m_graphicsSystem->initialize(setting.Hwnd);
	}

	void Engine::uninitialize()
	{
		m_logManager->uninitialize();
		m_eventSystem->uninitialize();
		m_threadPool->uninitialize();
		m_graphicsSystem->uninitialize();
	}

	void Engine::update()
	{
		m_timer->update();
		m_eventSystem->dispatchEvent(EventType::Update, m_timer->deltaTime());
		m_graphicsSystem->draw();
	}
}