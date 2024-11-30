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
		enum RendererCategory
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
		
		RendererCategory getRendererCategory();
		void setRendererCategory(RendererCategory rendererCategory);

		void fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters);
	private:
		std::shared_ptr<Renderer>		m_renderer;
		std::shared_ptr<RenderStates>	m_renderStates;
		RendererCategory m_rendererCategory;
	};

	inline std::shared_ptr<Renderer> RenderPass::getRenderer()
	{
		return m_renderer;
	}

	inline std::shared_ptr<RenderStates> RenderPass::getRenderStates()
	{
		return m_renderStates;
	}

	inline RenderPass::RendererCategory RenderPass::getRendererCategory()
	{
		return m_rendererCategory;
	}

	inline void RenderPass::setRendererCategory(RendererCategory rendererCategory)
	{
		m_rendererCategory = rendererCategory;
	}
}