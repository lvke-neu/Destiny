#include "Scene3D.h"
#include "Node3D.h"
#include "Component.h"

namespace Destiny
{
	Scene3D::Scene3D() 
	{
		m_root = std::make_shared<Node3D>();
		m_root->set_name("root");

		auto comp1 = std::make_shared<Component>();
		m_root->addComponent(comp1);

		auto node1 = std::make_shared<Node3D>();
		node1->set_name("node1");
		node1->addToParent(m_root);
		auto node2 = std::make_shared<Node3D>();
		node2->set_name("node2");
		node2->addToParent(m_root);
		auto node3 = std::make_shared<Node3D>();
		node3->set_name("node3");
		node3->addToParent(m_root);

		auto node11 = std::make_shared<Node3D>();
		node11->set_name("node11");
		node11->addToParent(node1);
		auto node12 = std::make_shared<Node3D>();
		node12->set_name("node12");
		node12->addToParent(node1);
		auto node13 = std::make_shared<Node3D>();
		node13->set_name("node13");
		node13->addToParent(node1);
	}
}