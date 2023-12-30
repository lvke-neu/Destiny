#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class BoxComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
		friend class RenderToTextureComponent;
	public:
		BoxComponent();
	public:
		GET_CLASS_NAME(BoxComponent);
	};
}