#pragma once
#include "Engine/Asset.h"
#include <d3d11.h>

namespace Destiny
{
	class SamplerState : public Asset
	{
	public:
		static D3D11_SAMPLER_DESC Default_SamplerState_Desc;
	public:
		SamplerState();
		virtual ~SamplerState();
	public:
		virtual void doLoad() override;
		ID3D11SamplerState* getSamplerState();
	private:
		ID3D11SamplerState* m_samplerState;
	};

	inline ID3D11SamplerState* SamplerState::getSamplerState()
	{
		return m_samplerState;
	}
}