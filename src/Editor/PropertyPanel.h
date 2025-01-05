#pragma once
#include "Meta.h"
#include <rttr/property.h>

namespace Destiny
{
	class Node;
	class Object;
}

class PropertyPanel : public Meta
{
public:
	PropertyPanel();
public:
	void update();
public slots:
	void onChoosedNode(void* parameter);
private:
	void reflect(std::shared_ptr<Destiny::Object> object);
	void reflectProperty(const rttr::property& property, std::shared_ptr<Destiny::Object> object);

	void reflectString(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectFloat(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
private:
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
