#pragma once
#include "Scene/NCS/VisualComponent.h"

namespace Destiny
{
	class SizeBillboardComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		SizeBillboardComponent();
		virtual ~SizeBillboardComponent() = default;
	public:
		GET_CLASS_NAME(PixelBillboardComponent);
	};
}