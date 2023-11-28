#include "Scene3D.h"
#include "Node3D.h"
#include "Component.h"
#include "Visual3DComponent.h"
#include "BoxComponent.h"
#include "Camera3DComponent.h"

namespace Destiny
{
	Scene3D::Scene3D() 
	{
		m_rootNode = std::make_shared<Node3D>();
		m_rootNode->set_name("Root");

		m_cameraNode = std::make_shared<Node3D>();
		m_cameraNode->set_name("Camera");
		m_cameraNode->addToParent(m_rootNode);
		auto camera3DComponent = std::make_shared<Camera3DComponent>();
		m_cameraNode->addComponent(camera3DComponent);

		auto boxNode = std::make_shared<Node3D>();
		boxNode->set_name("Box");
		boxNode->addToParent(m_rootNode);
		auto boxComponent = std::make_shared<BoxComponent>();
		boxNode->addComponent(boxComponent);
	}
}