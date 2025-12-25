#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class TorusComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TorusComponent();
		virtual ~TorusComponent();
	public:
		virtual void onUpdate(float deltaTime) override;
	};
}