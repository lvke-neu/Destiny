#pragma once
#include "GraphicsCommandList.h"

namespace Destiny
{
	class RenderSystem;
	class GraphicsPipeline : public GraphicsCommandList
	{
	public:
		GraphicsPipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~GraphicsPipeline() = default;
	public:
		virtual void syncState();
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	protected:
		std::shared_ptr<RenderSystem> m_renderSystem;
	};
}