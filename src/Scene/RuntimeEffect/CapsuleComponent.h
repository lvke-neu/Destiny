#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class CapsuleComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		CapsuleComponent();
		virtual ~CapsuleComponent();
	public:
		virtual void onUpdate(float deltaTime) override;
	};
}