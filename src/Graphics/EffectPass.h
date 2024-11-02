#pragma once
#include <memory>

namespace Destiny
{
	class Renderer;
	class RenderStates;
	class DrawParameters;
	class EffectPass
	{
	public:
		EffectPass();
	public:
		std::shared_ptr<Renderer> getRenderer();
		std::shared_ptr<RenderStates> getRenderStates();

		void setRenderer(std::shared_ptr<Renderer> renderer);
		void setRenderStates(std::shared_ptr<RenderStates> renderStates);

		void fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters);
	private:
		std::shared_ptr<Renderer>		m_renderer;
		std::shared_ptr<RenderStates>	m_renderStates;
	};

	inline std::shared_ptr<Renderer> EffectPass::getRenderer()
	{
		return m_renderer;
	}

	inline std::shared_ptr<RenderStates> EffectPass::getRenderStates()
	{
		return m_renderStates;
	}
}