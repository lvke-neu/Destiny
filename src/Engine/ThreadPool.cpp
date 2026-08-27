#include "ThreadPool.h"

namespace Destiny
{
	void ThreadPool::initialize(int threadCount)
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		if (!m_threadPool.empty())
		{
			return;
		}

		m_stoped.store(false, std::memory_order_release);
		m_undo.store(0, std::memory_order_release);
		createThreadPool(threadCount);
	}

	void ThreadPool::uninitialize()
	{
		{
			std::lock_guard<std::mutex> lock{ m_lock };
			m_stoped.store(true, std::memory_order_release);
		}

		m_cv.notify_all();
		for (std::thread& thread : m_threadPool)
		{
			if (thread.joinable())
				thread.join();
		}

		std::lock_guard<std::mutex> lock{ m_lock };
		m_threadPool.clear();
	}

	void ThreadPool::createThreadPool(int threadCount)
	{
		threadCount = threadCount < 1 ? 1 : threadCount;
		for (int i = 0; i < threadCount; i++)
		{
			m_threadPool.emplace_back(
				[this]() {
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock{ this->m_lock };
							this->m_cv.wait(lock, [this]() {return this->m_stoped.load() || !this->m_tasks.empty(); });
							if (this->m_tasks.empty())
							{
								if (this->m_stoped.load())
								{
									return;
								}

								continue;
							}

							task = std::move(this->m_tasks.front());
							this->m_tasks.pop();
						}

						try
						{
							if (task)
							{
								task();
							}
						}
						catch (...)
						{
							// Keep the worker alive. The task owner is responsible for
							// publishing its own failure state.
						}

						this->m_undo.fetch_sub(1, std::memory_order_release);
					}
				}
			);
		}
	}

	void ThreadPool::commitTask(const std::function<void()>& task)
	{
		if (!task)
			return;

		std::lock_guard<std::mutex> lock{ m_lock };
		if (m_stoped.load(std::memory_order_acquire) || m_threadPool.empty())
			return;

		m_tasks.push(task);
		m_undo.fetch_add(1, std::memory_order_release);
		m_cv.notify_one();
	}

	bool ThreadPool::finished()
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		return m_tasks.empty() && m_undo.load(std::memory_order_acquire) == 0;
	}

}

