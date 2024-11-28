#pragma once
#include <memory>

namespace Destiny
{
	class Renderer;
	class RenderStates;
	class DrawParameters;
	class RenderPass
	{
	public:
		enum Pipeline
		{
			None,
			ForwardOpaque,
			ForwardTransparent
		};
	public:
		RenderPass();
	public:
		std::shared_ptr<Renderer> getRenderer();
		void setRenderer(std::shared_ptr<Renderer> renderer);

		std::shared_ptr<RenderStates> getRenderStates();
		void setRenderStates(std::shared_ptr<RenderStates> renderStates);
		
		Pipeline getPipeline();
		void setPipeline(Pipeline pipeline);

		void fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters);
	private:
		std::shared_ptr<Renderer>		m_renderer;
		std::shared_ptr<RenderStates>	m_renderStates;
		Pipeline m_pipeline;
	};

	inline std::shared_ptr<Renderer> RenderPass::getRenderer()
	{
		return m_renderer;
	}

	inline std::shared_ptr<RenderStates> RenderPass::getRenderStates()
	{
		return m_renderStates;
	}

	inline RenderPass::Pipeline RenderPass::getPipeline()
	{
		return m_pipeline;
	}

	inline void RenderPass::setPipeline(Pipeline pipeline)
	{
		m_pipeline = pipeline;
	}
}