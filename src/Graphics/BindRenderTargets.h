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
		virtual ~BindRenderTargets();
	public:
		std::shared_ptr<RenderTargetView> getRenderTargetView();
		std::shared_ptr<DepthStencilView> getDepthStencilView();
		void	setRenderTargetView(unsigned int width, unsigned int height);
		void	setDepthStencilView(unsigned int width, unsigned int height);
		void	setViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float naxDepth);
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	private:
		std::shared_ptr<RenderTargetView>		m_renderTargetView;
		std::shared_ptr<DepthStencilView>		m_depthStencilView;
		std::shared_ptr<D3D11_VIEWPORT>			m_viewPort;
	};

	inline std::shared_ptr<RenderTargetView> BindRenderTargets::getRenderTargetView()
	{
		return m_renderTargetView;
	}

	inline std::shared_ptr<DepthStencilView> BindRenderTargets::getDepthStencilView()
	{
		return m_depthStencilView;
	}
}