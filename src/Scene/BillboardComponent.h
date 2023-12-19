#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class BillboardComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		BillboardComponent();
	public:
		GET_CLASS_NAME(BillboardComponent);
	};
}