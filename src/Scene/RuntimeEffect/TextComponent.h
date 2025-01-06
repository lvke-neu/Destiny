#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color32.h"

namespace Destiny
{
	class TextComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TextComponent();
		virtual ~TextComponent() = default;
	public:
		GET(std::string, text);
		void set_text(std::string text);
		GET(DirectX::XMFLOAT2, size);
		void set_size(DirectX::XMFLOAT2 size);
		GET(DirectX::XMFLOAT2, screenPosition);
		void set_screenPosition(DirectX::XMFLOAT2 screenPosition);
		GET(Color32, color);
		void set_color(Color32 color);
	private:
		std::string m_text;
		DirectX::XMFLOAT2 m_size;
		DirectX::XMFLOAT2 m_screenPosition;
		Color32 m_color;
	};
}