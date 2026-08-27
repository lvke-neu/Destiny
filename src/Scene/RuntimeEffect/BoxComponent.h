#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class BoxComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		BoxComponent();
		virtual ~BoxComponent();
	public:
		virtual void onUpdate(float deltaTime) override;
	};
}