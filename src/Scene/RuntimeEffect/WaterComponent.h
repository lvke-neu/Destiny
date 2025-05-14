#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class WaterComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		WaterComponent();
		virtual ~WaterComponent() = default;
	public:
		GET(float, width);
		void set_width(float width);

		GET(float, depth);
		void set_depth(float depth);

		GET(unsigned int, m);
		void set_m(unsigned int m);

		GET(unsigned int, n);
		void set_n(unsigned int n);

		GET(DirectX::XMFLOAT4, waveParams1);
		void set_waveParams1(DirectX::XMFLOAT4 waveParams1);

		GET(DirectX::XMFLOAT4, waveParams2);
		void set_waveParams2(DirectX::XMFLOAT4 waveParams2);

		GET(DirectX::XMFLOAT4, waveParams3);
		void set_waveParams3(DirectX::XMFLOAT4 waveParams3);

		GET(Color, waveColor);
		void set_waveColor(Color waveColor);

		GET_SET(float, waveSpeed);

		GET(float, waveScale);
		void set_waveScale(float waveScale);

		GET(float, waveLevel);
		void set_waveLevel(float waveLevel);
	public:
		virtual void onUpdate(float deltaTime) override;
	private:
		float m_width;
		float m_depth;
		unsigned int m_m;
		unsigned int m_n;

		float m_sumTime;
		DirectX::XMFLOAT4 m_waveParams1;
		DirectX::XMFLOAT4 m_waveParams2;
		DirectX::XMFLOAT4 m_waveParams3;
		Color m_waveColor;
		float m_waveSpeed;
		float m_waveScale;
		float m_waveLevel;
	};
}