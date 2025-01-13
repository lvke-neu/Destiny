#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	enum class CircularMotion
	{
		None,
		X_Z,
		Y_Z,
		X_Y
	};

	class ScriptComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		ScriptComponent();
		virtual ~ScriptComponent() = default;
	public:
		GET_SET(float, speed);
		GET_SET(float, length);
		GET_SET(CircularMotion, circularMotion);
	public:
		virtual void onUpdate(float deltaTime) override;
	private:
		float m_speed;
		float m_length;
		CircularMotion m_circularMotion;
	};
}