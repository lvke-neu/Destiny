#include "SceneManager.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	SceneManager::SceneManager()
	{

	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::initialize()
	{
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		auto deferredContext = Engine::GetInstance()->getGraphicsSystem()->getDeferredContext();
		auto rtv = Engine::GetInstance()->getGraphicsSystem()->getRenderTargetView();
		auto dsv = Engine::GetInstance()->getGraphicsSystem()->getDepthStencilView();

		float color[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
		deferredContext->ClearRenderTargetView(rtv, color);
		deferredContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		deferredContext->FinishCommandList(false, &m_commandList);
		Engine::GetInstance()->getGraphicsSystem()->addCommandList(m_commandList);
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::tick()
	{
	}
}