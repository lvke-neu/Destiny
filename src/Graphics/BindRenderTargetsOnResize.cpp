#include "BindRenderTargetsOnResize.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

namespace Destiny
{
	BindRenderTargetsOnResize::BindRenderTargetsOnResize()
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&BindRenderTargetsOnResize::onResize, this, std::placeholders::_1));
	}

	BindRenderTargetsOnResize::~BindRenderTargetsOnResize()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&BindRenderTargetsOnResize::onResize, this, std::placeholders::_1));
	}

	void BindRenderTargetsOnResize::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}

		setRenderTargetView(std::make_shared<Destiny::RenderTargetView>(wrd.width, wrd.height));
		getRenderTargetView()->load(0);
		setDepthStencilView(std::make_shared<Destiny::DepthStencilView>(wrd.width, wrd.height));
		getDepthStencilView()->load(0);
		setViewport(0.0f, 0.0f, (float)wrd.width, (float)wrd.height, 0.0f, 1.0f);
	}
}