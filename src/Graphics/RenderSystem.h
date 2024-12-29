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
		void									addBeforePipelineCommand(std::shared_ptr<GraphicsCommand> graphicsCommand);
		void									commitVisual(std::shared_ptr<Pre_Post_Process_Command> pre_post_process_command, std::shared_ptr<Visual> visual);
	private:
		std::shared_ptr<GraphicsPipeline>		m_forwardOpaquePipeline;
		std::shared_ptr<GraphicsPipeline>		m_forwardTransparentPipeline;
		std::shared_ptr<GraphicsCommandList>	m_beforePipelineCommandList;
	};

}