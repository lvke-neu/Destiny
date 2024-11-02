#pragma once
#include "GraphicsPipeline.h"

namespace Destiny 
{
	class RenderTargetView;
	class BindRenderTargets;
	class ForwardOpaquePipeline : public GraphicsPipeline
	{
	public:
		ForwardOpaquePipeline();
		virtual ~ForwardOpaquePipeline();
	public:
		void syncState();
	public:
		std::shared_ptr<RenderTargetView> getRenderTargetView();
	private:
		void onResize(void* data);
	private:
		std::shared_ptr<BindRenderTargets> m_bindRenderTargets;
	};
}