#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class ScriptComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		ScriptComponent();
		virtual ~ScriptComponent() = default;
	public:
		GET_SET(float, rotationSpeed);
	public:
		virtual void onUpdate(float deltaTime) override;
	private:
		float m_rotationSpeed;
	};
}