#include "ForwardOpaquePipeline.h"
#include "BindRenderTargets.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

namespace Destiny
{
	ForwardOpaquePipeline::ForwardOpaquePipeline() :
		m_bindRenderTargets(std::make_shared<BindRenderTargets>())
	{
		addRenderCommand(m_bindRenderTargets);
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&ForwardOpaquePipeline::onResize, this, std::placeholders::_1));
	}

	ForwardOpaquePipeline::~ForwardOpaquePipeline()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&ForwardOpaquePipeline::onResize, this, std::placeholders::_1));
	}

	std::shared_ptr<RenderTargetView> ForwardOpaquePipeline::getRenderTargetView()
	{
		return m_bindRenderTargets ? m_bindRenderTargets->getRenderTargetView() : nullptr;
	}

	void ForwardOpaquePipeline::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}

		m_bindRenderTargets->setRenderTargetView(std::make_shared<Destiny::RenderTargetView>(wrd.width, wrd.height));
		m_bindRenderTargets->getRenderTargetView()->load(0);
		m_bindRenderTargets->setDepthStencilView(std::make_shared<Destiny::DepthStencilView>(wrd.width, wrd.height));
		m_bindRenderTargets->getDepthStencilView()->load(0);
		m_bindRenderTargets->setViewport(0.0f, 0.0f, (float)wrd.width, (float)wrd.height, 0.0f, 1.0f);
	}
}