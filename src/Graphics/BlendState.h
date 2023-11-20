#pragma once
#include "Engine/Asset.h"
#include <d3d11.h>

namespace Destiny
{
	class BlendState : public Asset
	{
	public:
		static D3D11_BLEND_DESC Default_BlendState_Desc;
	public:
		BlendState();
		virtual ~BlendState();
	public:
		virtual void doLoad() override;
		ID3D11BlendState* getBlendState();
	private:
		ID3D11BlendState* m_blendState;
	};

	inline ID3D11BlendState* BlendState::getBlendState()
	{
		return m_blendState;
	}
}