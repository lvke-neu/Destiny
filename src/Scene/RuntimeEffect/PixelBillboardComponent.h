#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class PixelBillboardComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		PixelBillboardComponent();
		virtual ~PixelBillboardComponent() = default;
	public:
		GET(DirectX::XMFLOAT2, size);
		void set_size(DirectX::XMFLOAT2 size);
		GET(std::string, texturePath);
		void set_texturePath(std::string texturePath);
		GET(Color, color);
		void set_color(Color color);
	private:
		DirectX::XMFLOAT2 m_size;
		std::string m_texturePath;
		Color	m_color;
	};
}