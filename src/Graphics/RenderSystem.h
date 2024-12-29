#pragma once
#include "GraphicsSystem.h"

namespace Destiny
{
	class GraphicsCommand;
	class GraphicsCommandList;
	class GraphicsPipeline;
	class RenderSystem : public GraphicsSystem
	{
	public:
		virtual ~RenderSystem() = default;
	public:
		virtual void	createPipeline() override;
		virtual void	render() override;
		virtual void	syncState() override;
		virtual void	commitVisual(std::shared_ptr<Visual> visual) override;
	public:
		void			addBeforeForwardOpaqueCommand(std::shared_ptr<GraphicsCommand> graphicsCommand);
		void			addBeforeForwardTransparentCommandList(std::shared_ptr<GraphicsCommand> graphicsCommand);
	private:
		std::shared_ptr<GraphicsPipeline>				m_forwardOpaquePipeline;
		std::shared_ptr<GraphicsPipeline>				m_forwardTransparentPipeline;
		
		std::shared_ptr<GraphicsCommandList>			m_beforeForwardOpaqueCommandList;
		std::shared_ptr<GraphicsCommandList>			m_beforeForwardTransparentCommandList;
	};

}