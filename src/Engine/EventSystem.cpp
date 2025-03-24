#include "EventSystem.h"

namespace Destiny
{
	EventSystem::EventSystem() :
		m_viewportHovered(false)
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
		if (type == EventType::KeyPressed)
		{
			m_keyTriggers[*(KeyCode*)data] = true;
		}
		else if (type == EventType::KeyReleased)
		{
			m_keyTriggers[*(KeyCode*)data] = false;

		}
		if (type == EventType::MousePressed)
		{
			m_MouseTriggers[*(MouseCode*)data] = true;
		}
		else if (type == EventType::MouseReleased)
		{
			m_MouseTriggers[*(MouseCode*)data] = false;
		}
		if (type == EventType::MousePressed || type == EventType::MouseMoved || type == EventType::MouseReleased)
		{
			if (!m_viewportHovered)
			{
				return;
			}
		}
		for (const auto& event : m_events[type])
		{
			event(data);
		}
	}

	bool EventSystem::isKeyPressed(KeyCode keyCode)
	{
		return m_keyTriggers[keyCode];
	}

	bool EventSystem::isMousePressed(MouseCode mouseCode)
	{
		return m_MouseTriggers[mouseCode];
	}
}