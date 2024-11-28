#pragma once
#include "GraphicsSystem.h"
#include <memory>

namespace Destiny
{
	class ForwardOpaquePipeline;
	class ForwardTransparentPipeline;
	class RenderSystem : public GraphicsSystem
	{
	public:
		virtual ~RenderSystem() = default;
	public:
		virtual void createPipeline() override;
		virtual void render() override;
		virtual void syncState() override;
		virtual void commitVisual(std::shared_ptr<Visual> visual) override;
	public:
		std::shared_ptr<ForwardOpaquePipeline> getForwardOpaquePipeline();
	private:
		std::shared_ptr<ForwardOpaquePipeline> m_forwardOpaquePipeline;
		std::shared_ptr<ForwardTransparentPipeline> m_forwardTransparentPipeline;
	};

	inline std::shared_ptr<ForwardOpaquePipeline> RenderSystem::getForwardOpaquePipeline()
	{
		return m_forwardOpaquePipeline;
	}
}