#pragma once
#include "GraphicsPipeline/GraphicsCommand.h"
#include <memory>

struct D3D11_VIEWPORT;
namespace Destiny
{
	class RenderTargetView;
	class DepthStencilView;
	class BindRenderTargets : public GraphicsCommand
	{
	public:
		BindRenderTargets();
		virtual ~BindRenderTargets() = default;
	public:
		void									setRenderTargetView(std::shared_ptr<RenderTargetView> renderTargetView);
		std::shared_ptr<RenderTargetView>		getRenderTargetView();
		void									setDepthStencilView(std::shared_ptr<DepthStencilView> depthStencilView);
		std::shared_ptr<DepthStencilView>		getDepthStencilView();
		void									setViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float naxDepth);
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	private:
		std::shared_ptr<RenderTargetView>		m_renderTargetView;
		std::shared_ptr<DepthStencilView>		m_depthStencilView;
		std::shared_ptr<D3D11_VIEWPORT>			m_viewPort;

	};
}