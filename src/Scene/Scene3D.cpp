#include "Scene3D.h"
#include "Node3D.h"

namespace Destiny
{
	Scene3D::Scene3D() 
	{
		m_root = std::make_shared<Node3D>();
		m_root->setName("root");

		auto node1 = std::make_shared<Node3D>();
		node1->setName("node1");
		node1->addToParent(m_root);
		auto node2 = std::make_shared<Node3D>();
		node2->setName("node2");
		node2->addToParent(m_root);
		auto node3 = std::make_shared<Node3D>();
		node3->setName("node3");
		node3->addToParent(m_root);

		auto node11 = std::make_shared<Node3D>();
		node11->setName("node11");
		node11->addToParent(node1);
		auto node12 = std::make_shared<Node3D>();
		node12->setName("node12");
		node12->addToParent(node1);
		auto node13 = std::make_shared<Node3D>();
		node13->setName("node13");
		node13->addToParent(node1);
	}
}