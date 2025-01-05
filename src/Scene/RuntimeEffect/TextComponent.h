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
		void updateText(const std::string& text);
		void updateSize(float size_x, float size_y);
		void updateScreenPosition(float screen_x, float screen_y);
		void updateTextColor(float r, float g, float b);
	};
}