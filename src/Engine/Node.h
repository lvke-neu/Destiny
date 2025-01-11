#pragma once
#include "Engine/Object.h"
#include "Math/Transform.h"

namespace Destiny
{
	class Component;
	class Node : public Object, public std::enable_shared_from_this<Node>
	{
		RTTR_ENABLE(Object);
	public:
		Node();
		Node(const std::string& name);
		virtual ~Node() = default;
	public:
		std::shared_ptr<Node>								getParent();
		void												addToParent(std::shared_ptr<Node> parent);
		void												removeFromParent();
		void												addComponent(std::shared_ptr<Component> component);
		void												removeComponent(std::shared_ptr<Component> component);
		const std::vector<std::shared_ptr<Component>>&		getComponents();
		const std::vector<std::shared_ptr<Node>>&			getChilds();
	public:
		void												moveZAxis(float distance);
		void												moveXAxis(float distance);
		void												rotateXAxis(float angle);
		void												rotateYAxis(float angle);
		DirectX::XMMATRIX									getRootToThisWorldMatrix();
	public:
		GET_SET(std::string, name);
		GET(Transform, transform);
		void												set_transform(Transform transform);
	private:
		void												onEnterScene();
		void												onLeaveScene();
		void												onNodeTransformChanged();
	protected:
		std::string											m_name;
		std::shared_ptr<Node>								m_parent;
		std::vector<std::shared_ptr<Node>>					m_childs;
		std::vector<std::shared_ptr<Component>>				m_components;
		Transform											m_transform;
	};

	inline std::shared_ptr<Node> Node::getParent()
	{
		return m_parent;
	}

	inline const std::vector<std::shared_ptr<Component>>& Node::getComponents()
	{
		return m_components;
	}

	inline const std::vector<std::shared_ptr<Node>>& Node::getChilds()
	{
		return m_childs;
	}
}
