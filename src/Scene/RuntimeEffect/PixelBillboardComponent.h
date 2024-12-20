#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class PixelBillboardComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		PixelBillboardComponent() = default;
		PixelBillboardComponent(const char* texturePath);
		virtual ~PixelBillboardComponent() = default;
	public:
		GET_CLASS_NAME(PixelBillboardComponent);
	};
}