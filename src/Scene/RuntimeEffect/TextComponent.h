#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class TextComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TextComponent();
		virtual ~TextComponent() = default;
	public:
		GET_CLASS_NAME(TextComponent);
	};
}