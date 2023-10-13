#include "EventSystem.h"

namespace Destiny
{
	EventSystem::EventSystem()
	{

	}

	EventSystem::~EventSystem()
	{
		m_events.clear();
	}

	void EventSystem::registerEvent(EventType type, Event event)
	{
		m_events[type].push_back(event);
	}

	void EventSystem::unRegisterEvent(EventType type, Event event)
	{
		for (auto iter = m_events[type].begin(); iter != m_events[type].end(); iter++)
		{
			if (iter->target_type() == event.target_type())
			{
				m_events[type].erase(iter);
				break;
			}
		}
	}

	void EventSystem::dispatchEvent(EventType type, void* data)
	{
		for (const auto& event : m_events[type])
		{
			event(data);
		}
	}
}