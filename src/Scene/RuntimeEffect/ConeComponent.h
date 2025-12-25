#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class ConeComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		ConeComponent();
		virtual ~ConeComponent();
	public:
		virtual void onUpdate(float deltaTime) override;
	};
}