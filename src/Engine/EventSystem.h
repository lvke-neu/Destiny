#pragma once
#include <functional>
#include <unordered_map>
#include <vector>

namespace Destiny
{
	typedef std::function<void(void*)> Event;

	enum class EventType
	{
		WindowResize
	};

	struct WindowResizeData
	{
		unsigned int width;
		unsigned int height;
	};

	class EventSystem
	{
	public:
		EventSystem();
		~EventSystem();
	public:
		void initialize() {}
		void uninitialize() {}
	public:
		void registerEvent(EventType type, Event event);
		void unRegisterEvent(EventType type, Event event);
		void dispatchEvent(EventType type, void* data);
	private:
		std::unordered_map<EventType, std::vector<Event>> m_events;
	};
}