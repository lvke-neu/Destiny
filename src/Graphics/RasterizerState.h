#pragma once
#include "Engine/Asset.h"
#include <d3d11.h>

namespace Destiny
{
	class RasterizerState : public Asset
	{
	public:
		static D3D11_RASTERIZER_DESC Default_Rasterizer_Desc;
	public:
		RasterizerState();
		virtual ~RasterizerState();
	public:
		virtual void doLoad() override;
		ID3D11RasterizerState* getRasterizerState();
	private:
		ID3D11RasterizerState* m_rasterizerState;
	};

	inline ID3D11RasterizerState* RasterizerState::getRasterizerState()
	{
		return m_rasterizerState;
	}
}