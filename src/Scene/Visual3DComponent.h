#pragma once
#include "Component.h"
#include "Graphics/RasterizerState.h"
#include "Graphics/DepthStencilState.h"
#include "Graphics/Color.h"
#include "Graphics/ConstantBuffer.h"
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;

	class Visual3D;
	class Visual3DComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		Visual3DComponent();
	public:
		virtual void onAttachNode() override;
		virtual void onNodeTransformChanged() override;
		void setWorldMatrix();
	public:
		GET_CLASS_NAME(Visual3DComponent);
		GET(std::shared_ptr<Visual3D>, visual3D);
		GET(D3D11_RASTERIZER_DESC, rasterizerStateDesc);
		GET_SET(D3D11_DEPTH_STENCIL_DESC, depthStencilStateDesc);
		GET_SET(Color, color);
		void set_rasterizerStateDesc(D3D11_RASTERIZER_DESC rasterizerStateDesc);
	protected:
		std::shared_ptr<Visual3D> m_visual3D;
		D3D11_RASTERIZER_DESC m_rasterizerStateDesc;
		D3D11_DEPTH_STENCIL_DESC m_depthStencilStateDesc;
		Color m_color;

		std::shared_ptr<ConstantBuffer<XMMATRIX>> m_worldMatrix;
	};
}