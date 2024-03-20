#include "Engine.h"
#include "LogManager.h"
#include "BlobLoaderManager.h"
#include "EventSystem.h"
#include "ThreadPool.h"
#include "Timer.h"
#include "Graphics/GraphicsSystem.h"
#include "Scene/SceneManager.h"

namespace Destiny
{
	Engine::Engine()
	{
		m_timer = std::make_shared<Timer>();
		m_logManager = std::make_shared<LogManager>();
		m_blobLoaderManager = std::make_shared<BlobLoaderManager>();
		m_eventSystem = std::make_shared<EventSystem>();
		m_dataLoadThreadPool = std::make_shared<ThreadPool>();
		m_graphicsSystem = std::make_shared<GraphicsSystem>();
		m_sceneManager = std::make_shared<SceneManager>();
	}

	void Engine::initialize(const EngineSetting& setting)
	{
		m_logManager->initialize();
		m_eventSystem->initialize();
		m_dataLoadThreadPool->initialize(setting.DataLoadingThreadCount);
		m_graphicsSystem->initialize(setting.Hwnd);
		m_sceneManager->initialize();
	}

	void Engine::uninitialize()
	{
		m_logManager->uninitialize();
		m_eventSystem->uninitialize();
		m_dataLoadThreadPool->uninitialize();
		m_graphicsSystem->uninitialize();
		m_sceneManager->uninitialize();
	}

	void Engine::update()
	{
		m_timer->update();
		m_eventSystem->dispatchEvent(EventType::Update, m_timer->deltaTime());
		m_sceneManager->update();
	}
}