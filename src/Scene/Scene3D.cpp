#include "Scene3D.h"
#include "Node3D.h"
#include "Component.h"
#include "Visual3DComponent.h"
#include "BoxComponent.h"

namespace Destiny
{
	Scene3D::Scene3D() 
	{
		m_root = std::make_shared<Node3D>();
		m_root->set_name("Root");

		auto node1 = std::make_shared<Node3D>();
		node1->set_name("BaseScene");
		node1->addToParent(m_root);

		auto comp1 = std::make_shared<BoxComponent>();
		node1->addComponent(comp1);
	}
}