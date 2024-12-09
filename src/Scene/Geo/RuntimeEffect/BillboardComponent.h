#pragma once
#include "Scene/NCS/VisualComponent.h"

namespace Destiny
{
	class BillboardComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		BillboardComponent();
		virtual ~BillboardComponent() = default;
	public:
		GET_CLASS_NAME(BillboardComponent);
	};
}