#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

typedef std::function<void()> Slot;
class Meta
{
public:
	virtual ~Meta() = default;
public:
	static void connect(std::shared_ptr<Meta> sender, int signal, Slot slot);
protected:
	void connect(int signal, Slot slot);
	void send(int signal);
private:
	std::unordered_map<int, std::vector<Slot>> m_slots;
};
