#include "Meta.h"

void Meta::connect_(std::shared_ptr<Meta> sender, int signal, Slot slot)
{
	if (!sender)
	{
		return;
	}

	sender->connect_(signal, slot);
}

void Meta::connect_(int signal, Slot slot)
{
	auto& _slots = m_slots[signal];
	_slots.push_back(slot);
}

void Meta::send(int signal, void* parameter)
{
	auto iter = m_slots.find(signal);
	if (iter != m_slots.end())
	{
		for (const auto& slot : iter->second)
		{
			if (slot)
			{
				slot(parameter);
			}
		}
	}
}
