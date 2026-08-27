#pragma once
#include "Math/Color.h"
#include "GraphicsPipeline/GraphicsCommand.h"
#include <memory>

namespace Destiny
{
	class RenderTargetView;
	class DepthStencilView;
	class ClearRenderTarget : public GraphicsCommand
	{
	public:
		ClearRenderTarget();
		virtual ~ClearRenderTarget();
	public:
		void	setRenderTargetView(std::shared_ptr<RenderTargetView> renderTargetView);
		void	setDepthStencilView(std::shared_ptr<DepthStencilView> depthStencilView);
		void	setClearColor(const Color& color);
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	private:
		std::shared_ptr<RenderTargetView>		m_renderTargetView;
		std::shared_ptr<DepthStencilView>		m_depthStencilView;
		Color m_clearColor;
	};
}