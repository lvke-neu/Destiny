#pragma once
#include "Utility.h"
#include <memory>

namespace Destiny
{
	struct EngineSetting
	{
		long long Hwnd;
		int DataLoadingThreadCount;
	};

	class Timer;
	class LogManager;
	class BlobLoaderManager;
	class EventSystem;
	class ThreadPool;
	class GraphicsSystem;
	class SceneManager;
	class PhysicsSystem;
	class Engine
	{
		SINGLETON(Engine);
	private:
		Engine();
		~Engine() = default;
		Engine(const Engine&) = default;
		Engine& operator=(const Engine&) = default;
	public:
		void initialize(const EngineSetting& setting);
		void uninitialize();
		void update();
	public:
		std::shared_ptr<LogManager>			getLogManager();
		std::shared_ptr<BlobLoaderManager>	getBlobLoaderManager();
		std::shared_ptr<EventSystem>		getEventSystem();
		std::shared_ptr<ThreadPool>			getThreadPool();
		std::shared_ptr<GraphicsSystem>		getGraphicsSystem();
		std::shared_ptr<SceneManager>		getSceneManager();
		std::shared_ptr<PhysicsSystem>		getPhysicsSystem();
	private:
		std::shared_ptr<Timer>				m_timer;
		std::shared_ptr<LogManager>			m_logManager;
		std::shared_ptr<BlobLoaderManager>	m_blobLoaderManager;
		std::shared_ptr<EventSystem>		m_eventSystem;
		std::shared_ptr<ThreadPool>			m_dataLoadThreadPool;
		std::shared_ptr<GraphicsSystem>		m_graphicsSystem;
		std::shared_ptr<SceneManager>		m_sceneManager;
		std::shared_ptr<PhysicsSystem>		m_physicsSystem;
	};

	inline std::shared_ptr<LogManager> Engine::getLogManager()
	{
		return m_logManager;
	}

	inline std::shared_ptr<BlobLoaderManager> Engine::getBlobLoaderManager()
	{
		return m_blobLoaderManager;
	}

	inline std::shared_ptr<EventSystem> Engine::getEventSystem()
	{
		return m_eventSystem;
	}

	inline std::shared_ptr<ThreadPool> Engine::getThreadPool()
	{
		return m_dataLoadThreadPool;
	}

	inline std::shared_ptr<GraphicsSystem> Engine::getGraphicsSystem()
	{
		return m_graphicsSystem;
	}

	inline std::shared_ptr<SceneManager> Engine::getSceneManager()
	{
		return m_sceneManager;
	}

	inline std::shared_ptr<PhysicsSystem> Engine::getPhysicsSystem()
	{
		return m_physicsSystem;
	}
}