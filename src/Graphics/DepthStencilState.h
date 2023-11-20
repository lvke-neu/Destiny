#pragma once
#include "Engine/Asset.h"
#include <d3d11.h>

namespace Destiny
{
	class DepthStencilState : public Asset
	{
	public:
		static D3D11_DEPTH_STENCIL_DESC Default_DepthStencil_Desc;
	public:
		DepthStencilState();
		virtual ~DepthStencilState();
	public:
		virtual void doLoad() override;
		ID3D11DepthStencilState* getDepthStencilState();
	private:
		ID3D11DepthStencilState* m_depthStencilState;
	};

	inline ID3D11DepthStencilState* DepthStencilState::getDepthStencilState()
	{
		return m_depthStencilState;
	}
}