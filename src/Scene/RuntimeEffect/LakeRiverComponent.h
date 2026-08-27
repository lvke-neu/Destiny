#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class BindRenderTargets;
	class ClearRenderTarget;

	class LakeRiverComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		LakeRiverComponent();
		virtual ~LakeRiverComponent();

	public:
		GET(float, width);
		void set_width(float width);

		GET(float, depth);
		void set_depth(float depth);

		GET(unsigned int, m);
		void set_m(unsigned int m);

		GET(unsigned int, n);
		void set_n(unsigned int n);

		// Wave parameters
		GET(DirectX::XMFLOAT4, waveParams1);
		void set_waveParams1(DirectX::XMFLOAT4 waveParams1);

		GET(DirectX::XMFLOAT4, waveParams2);
		void set_waveParams2(DirectX::XMFLOAT4 waveParams2);

		GET(DirectX::XMFLOAT4, waveParams3);
		void set_waveParams3(DirectX::XMFLOAT4 waveParams3);

		GET(Color, waveColor);
		void set_waveColor(Color waveColor);

		GET(float, waveScale);
		void set_waveScale(float waveScale);

		GET(float, waveLevel);
		void set_waveLevel(float waveLevel);

		// Flow Map parameters
		GET(DirectX::XMFLOAT2, flowSpeed);
		void set_flowSpeed(DirectX::XMFLOAT2 flowSpeed);

		GET(float, flowCycleScale);
		void set_flowCycleScale(float flowCycleScale);

		GET(float, flowStrength);
		void set_flowStrength(float flowStrength);

	public:
		virtual void onUpdate(float deltaTime) override;
		virtual void onEnterScene() override;

	public:
		std::shared_ptr<BindRenderTargets>		m_bindRenderTargets;

	private:
		void onResize(void* data);

	private:
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget;

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
		float m_waveScale;
		float m_waveLevel;

		DirectX::XMFLOAT2 m_flowSpeed;
		float m_flowCycleScale;
		float m_flowStrength;
	};
}
