#pragma once
#include "Scene/NCS/VisualComponent.h"

namespace Destiny
{
	class PixelBillboardComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		PixelBillboardComponent();
		virtual ~PixelBillboardComponent() = default;
	public:
		GET_CLASS_NAME(PixelBillboardComponent);
	};
}