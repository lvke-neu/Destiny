#pragma once
#include "GraphicsSystem.h"
#include <memory>

namespace Destiny
{
	class ForwardOpaquePipeline;
	class RenderSystem : public GraphicsSystem
	{
	public:
		virtual ~RenderSystem() = default;
	public:
		virtual void createPipeline() override;
		virtual void render() override;
		virtual void syncState() override;
	public:
		std::shared_ptr<ForwardOpaquePipeline> getForwardOpaquePipeline();
	private:
		std::shared_ptr<ForwardOpaquePipeline> m_forwardOpaquePipeline;
	};

	inline std::shared_ptr<ForwardOpaquePipeline> RenderSystem::getForwardOpaquePipeline()
	{
		return m_forwardOpaquePipeline;
	}
}