#pragma once
#include "GraphicsPipeline/GraphicsCommand.h"
#include <memory>

namespace Destiny
{
	class RenderTargetView;
	class DepthStencilView;
	class ClearRenderTargets : public GraphicsCommand
	{
	public:
		ClearRenderTargets();
		virtual ~ClearRenderTargets();
	public:
		void	setRenderTargetView(std::shared_ptr<RenderTargetView> renderTargetView);
		void	setDepthStencilView(std::shared_ptr<DepthStencilView> depthStencilView);
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	private:
		std::shared_ptr<RenderTargetView>		m_renderTargetView;
		std::shared_ptr<DepthStencilView>		m_depthStencilView;
	};
}