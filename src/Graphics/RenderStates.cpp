#include "RenderStates.h"
#include "GraphicsSystem.h"
#include "DrawParameters.h"
#include "Engine/Engine.h"
#include <d3d11.h>

namespace Destiny
{
	RenderStates::RenderStates() :
		m_rasterizerStateDesc(std::make_shared<CD3D11_RASTERIZER_DESC>(CD3D11_DEFAULT())),
		m_depthStencilStateDesc(std::make_shared<CD3D11_DEPTH_STENCIL_DESC>(CD3D11_DEFAULT())),
		m_blendStateDesc(std::make_shared<CD3D11_BLEND_DESC>(CD3D11_DEFAULT())),
		m_rasterizerState(nullptr),
		m_depthStencilState(nullptr),
		m_blendState(nullptr)
	{

	}

	RenderStates::~RenderStates()
	{
		SAFE_RELEASE(m_rasterizerState);
		SAFE_RELEASE(m_depthStencilState);
		SAFE_RELEASE(m_blendState);
	}

	void RenderStates::doLoad()
	{
		if (isLoadingSucceed())
		{
			return;
		}

		if (!m_rasterizerStateDesc || !m_depthStencilStateDesc || !m_blendStateDesc)
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, RenderStates load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}

		HRESULT hrRS = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateRasterizerState(m_rasterizerStateDesc.get(), &m_rasterizerState);
		HRESULT hrDSS = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateDepthStencilState(m_depthStencilStateDesc.get(), &m_depthStencilState);
		HRESULT hrBS = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBlendState(m_blendStateDesc.get(), &m_blendState);
		
		if (FAILED(hrRS))
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, RenderStates:RasterizerState load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}
		if (FAILED(hrDSS))
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, RenderStates:DepthStencilState load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}
		if (FAILED(hrBS))
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, RenderStates:BlendState load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}
		
		loadSucceeded__();
	}

	void RenderStates::fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters)
	{
		if (!drawParameters)
		{
			return;
		}

		drawParameters->rasterizerState = m_rasterizerState;
		drawParameters->depthStencilState = m_depthStencilState;
		drawParameters->blendState = m_blendState;
	}

	std::shared_ptr<RenderStates> RenderStates::CreateBlendState()
	{
		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();

		renderStates->getBlendStateDesc()->AlphaToCoverageEnable = true;
		renderStates->getBlendStateDesc()->IndependentBlendEnable = false;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendEnable = true;
		renderStates->getBlendStateDesc()->RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderStates->getBlendStateDesc()->RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		renderStates->getBlendStateDesc()->RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		renderStates->getBlendStateDesc()->RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		renderStates->getBlendStateDesc()->RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		renderStates->getDepthStencilStateDesc()->DepthEnable = false;

		return renderStates;
	}
}