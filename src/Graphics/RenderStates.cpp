#include "RenderStates.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	RenderStates::RenderStates(std::shared_ptr<D3D11_RASTERIZER_DESC> rasterizerStateDesc, std::shared_ptr <D3D11_DEPTH_STENCIL_DESC> depthStencilStateDesc, std::shared_ptr <D3D11_BLEND_DESC> blendStateDesc) :
		m_rasterizerStateDesc(rasterizerStateDesc),
		m_depthStencilStateDesc(depthStencilStateDesc),
		m_blendStateDesc(blendStateDesc),
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

	std::shared_ptr<RenderStates> RenderStates::Create_Default()
	{
		return std::make_shared<RenderStates>(std::shared_ptr<CD3D11_RASTERIZER_DESC>(new CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT())), std::shared_ptr<CD3D11_DEPTH_STENCIL_DESC>(new CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT())), std::shared_ptr<CD3D11_BLEND_DESC>(new CD3D11_BLEND_DESC(CD3D11_DEFAULT())));
	}
}