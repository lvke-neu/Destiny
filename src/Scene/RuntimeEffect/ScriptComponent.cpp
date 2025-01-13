#include "ScriptComponent.h"
#include "Engine/Node.h"
#include "Math/Math.h"

namespace Destiny
{
	ScriptComponent::ScriptComponent() :
		m_speed(1.0f),
		m_length(1.0f),
		m_circularMotion(CircularMotion::None),
		m_sum(0.0f)
	{

	}

	void ScriptComponent::onUpdate(float deltaTime)
	{
		if (!m_node || !m_enable)
		{
			return;
		}

		m_sum += deltaTime;
		if (m_sum > m_speed)
		{
			m_sum = 0.0f;
		}
		
		float radian = (m_sum / m_speed) * Math::PI * 2.0f ;

		auto transform = m_node->get_transform();
		auto translation = transform.get_translation();

		switch (m_circularMotion)
		{
		case CircularMotion::X_Z:
		{
			translation.x = std::cos(radian) * m_length;
			translation.z = std::sin(radian) * m_length;
			break;
		}
		case CircularMotion::Y_Z:
		{
			translation.y = std::cos(radian) * m_length;
			translation.z = std::sin(radian) * m_length;
			break;
		}
		case CircularMotion::X_Y:
		{
			translation.x = std::cos(radian) * m_length;
			translation.y = std::sin(radian) * m_length;
			break;
		}
		}


		
		transform.set_translation(translation);
		m_node->set_transform(transform);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ScriptComponent>("ScriptComponent")
			.constructor<>()
			.property("speed", &ScriptComponent::get_speed, &ScriptComponent::set_speed)
			.property("length", &ScriptComponent::get_length, &ScriptComponent::set_length)
			.property("circularMotion", &ScriptComponent::get_circularMotion, &ScriptComponent::set_circularMotion);
		rttr::registration::enumeration<CircularMotion>("CircularMotion")
		(
			rttr::value("None", CircularMotion::None),
			rttr::value("X_Z",  CircularMotion::X_Z),
			rttr::value("Y_Z",  CircularMotion::Y_Z),
			rttr::value("X_Y",  CircularMotion::X_Y)
		);
	}
}