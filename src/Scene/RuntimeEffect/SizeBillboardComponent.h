#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class SizeBillboardComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		SizeBillboardComponent();
		virtual ~SizeBillboardComponent() = default;
	public:
		void set_size(const DirectX::XMFLOAT2& size);
	};
}