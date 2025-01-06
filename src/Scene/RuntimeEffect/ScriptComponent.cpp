#include "ScriptComponent.h"
#include "Engine/Node.h"

namespace Destiny
{
	ScriptComponent::ScriptComponent() :
		m_rotationSpeed(100.0f)
	{

	}

	void ScriptComponent::onUpdate(float deltaTime)
	{
		if (!m_node || !m_enable)
		{
			return;
		}

		auto transform = m_node->get_transform();
		auto rotation = transform.get_rotation();
		rotation.y += deltaTime * m_rotationSpeed;
		transform.set_rotation(rotation);
		m_node->set_transform(transform);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ScriptComponent>("ScriptComponent")
			.constructor<>()
			.property("rotationSpeed", &ScriptComponent::get_rotationSpeed, &ScriptComponent::set_rotationSpeed);
	}
}