#pragma once
#include "Engine/Asset.h"

struct D3D11_RASTERIZER_DESC;
struct D3D11_DEPTH_STENCIL_DESC;
struct D3D11_BLEND_DESC;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
namespace Destiny
{
	class RenderParameters;
	class RenderStates : public Asset
	{
		friend class Effect;
	public:
		//Create_Default
		RenderStates();
		virtual ~RenderStates();
	public:
		virtual void doLoad() override;
	public:
		void fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters);
		std::shared_ptr<D3D11_RASTERIZER_DESC> getRasterizerStateDesc();
		std::shared_ptr<D3D11_DEPTH_STENCIL_DESC> getDepthStencilStateDesc();
		std::shared_ptr<D3D11_BLEND_DESC> getBlendStateDesc();
	private:
		std::shared_ptr<D3D11_RASTERIZER_DESC>		m_rasterizerStateDesc;
		std::shared_ptr<D3D11_DEPTH_STENCIL_DESC>	m_depthStencilStateDesc;
		std::shared_ptr<D3D11_BLEND_DESC>			m_blendStateDesc;
		ID3D11RasterizerState*						m_rasterizerState;
		ID3D11DepthStencilState*					m_depthStencilState;
		ID3D11BlendState*							m_blendState;
	};

	inline std::shared_ptr<D3D11_RASTERIZER_DESC> RenderStates::getRasterizerStateDesc()
	{
		return m_rasterizerStateDesc;
	}

	inline std::shared_ptr<D3D11_DEPTH_STENCIL_DESC> RenderStates::getDepthStencilStateDesc()
	{
		return m_depthStencilStateDesc;
	}

	inline std::shared_ptr<D3D11_BLEND_DESC> RenderStates::getBlendStateDesc()
	{
		return m_blendStateDesc;
	}
}