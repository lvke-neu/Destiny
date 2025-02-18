#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny
{
	class BindRenderTargets;
	class Visual;
	class RenderTargetView;
	class PostProcessPipeline : public GraphicsPipeline
	{
	public:
		PostProcessPipeline();
		virtual ~PostProcessPipeline();
	public:
		virtual void syncState() override;
	public:
		std::shared_ptr<RenderTargetView> getRTV();
	private:
		std::shared_ptr<BindRenderTargets> m_bindRenderTargets;
		std::shared_ptr<Visual> m_fullScreenTriangle;
	};
}