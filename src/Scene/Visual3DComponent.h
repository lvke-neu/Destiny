#pragma once
#include "Component.h"
#include "Graphics/RasterizerState.h"
#include "Graphics/DepthStencilState.h"
#include "Graphics/BlendState.h"
#include "Graphics/Color.h"
#include "Graphics/ConstantBuffer.h"
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;

	class Visual3D;
	class Material;
	class Visual3DComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		Visual3DComponent();
	public:
		virtual void onAttachNode() override;
		virtual void onNodeTransformChanged() override;
		void beforeDrawCommand();
	public:
		GET_CLASS_NAME(Visual3DComponent);
		GET(std::shared_ptr<Visual3D>, visual3D);
		GET(D3D11_RASTERIZER_DESC, rasterizerStateDesc);
		GET(D3D11_DEPTH_STENCIL_DESC, depthStencilStateDesc);
		GET(D3D11_BLEND_DESC, blendStateDesc);
		GET_SET(std::shared_ptr<Material>, material);
		GET(int, buttonRecompilePixelShader);
		GET(int, buttonRecompileGeometryShader);

		void set_rasterizerStateDesc(D3D11_RASTERIZER_DESC rasterizerStateDesc);
		void set_depthStencilStateDesc(D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc);
		void set_blendStateDesc(D3D11_BLEND_DESC blendStateDesc);
		void set_buttonRecompilePixelShader(int flag);
		void set_buttonRecompileGeometryShader(int flag);
		std::string get_pixelShaderPath();
		void set_pixelShaderPath(std::string)
		{

		}
	protected:
		std::shared_ptr<Visual3D> m_visual3D;
		std::shared_ptr<Material> m_material;
		D3D11_RASTERIZER_DESC m_rasterizerStateDesc;
		D3D11_DEPTH_STENCIL_DESC m_depthStencilStateDesc;
		D3D11_BLEND_DESC m_blendStateDesc;
		int m_buttonRecompilePixelShader;
		int m_buttonRecompileGeometryShader;

		struct CbWorld
		{
			XMMATRIX world;
			XMMATRIX worldInvTranspose;
		};
		std::shared_ptr<ConstantBuffer<CbWorld>> m_worldMatrix;
		struct MateriaColor
		{
			XMFLOAT4 ambientColor;
			XMFLOAT4 diffuseColor;
			XMFLOAT4 specularColor;
			XMFLOAT4 useColor;
		};
		std::shared_ptr<ConstantBuffer<MateriaColor>> m_materiaColor;
	};
}