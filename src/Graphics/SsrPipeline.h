#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny
{
	class BindRenderTargets;
	class Visual;
	class Texture;

	class SsrPipeline : public GraphicsPipeline
	{
	public:
		SsrPipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~SsrPipeline();

	public:
		virtual void execute(std::shared_ptr<GraphicsContext> deviceContext) override;
		void onRendererConstantChanged();

	private:
		void onResize(void* data);

	private:
		std::shared_ptr<Visual> m_fullScreenTriangle;
		std::shared_ptr<Texture> m_copyTexture;
	};
}
