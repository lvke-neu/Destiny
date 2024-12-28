#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

typedef std::function<void(void*)> Slot;
class Meta
{
public:
	virtual ~Meta() = default;
public:
	static void connect_(std::shared_ptr<Meta> sender, int signal, Slot slot);
protected:
	void connect_(int signal, Slot slot);
	void send(int signal, void* parameter);
private:
	std::unordered_map<int, std::vector<Slot>> m_slots;
};

#define MetaConnect(sender, signal, receiver, slot)\
Meta::connect_(sender, signal, std::bind(slot, receiver.get(), std::placeholders::_1));

#define slots