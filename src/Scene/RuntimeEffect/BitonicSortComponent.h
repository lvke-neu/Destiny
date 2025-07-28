#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class BitonicSortComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		BitonicSortComponent();
		virtual ~BitonicSortComponent() = default;
	};
}