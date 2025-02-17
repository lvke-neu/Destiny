#pragma once
#include "GraphicsSystem.h"
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny
{
	class Visual;
	class RenderSystem : public GraphicsSystem
	{
	public:
		virtual ~RenderSystem() = default;
	public:
		virtual void							createPipeline() override;
		virtual void							render() override;
		virtual void							syncState() override;
		void									commitVisual(std::shared_ptr<Visual> visual);
		std::shared_ptr<GraphicsPipeline>		getPostProcessPipeline();
	private:
		std::shared_ptr<GraphicsPipeline>		m_forwardOpaquePipeline;
		std::shared_ptr<GraphicsPipeline>		m_transparentPipeline;
		std::shared_ptr<GraphicsPipeline>		m_guiPipeline;
		std::shared_ptr<GraphicsPipeline>		m_postProcessPipeline;
	};

	inline std::shared_ptr<GraphicsPipeline> RenderSystem::getPostProcessPipeline()
	{
		return m_postProcessPipeline;
	}
}