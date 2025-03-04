#pragma once
#include "GraphicsPipeline/GraphicsCommand.h"
#include <vector>
#include <memory>

struct D3D11_VIEWPORT;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
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
		std::shared_ptr<RenderTargetView> getRenderTargetViews(unsigned int index);
		std::shared_ptr<DepthStencilView> getDepthStencilViews(unsigned int index);
		void	setRenderTargetViews(const std::vector<std::shared_ptr<RenderTargetView>>& renderTargetViews);
		void	setDepthStencilViews(const std::vector<std::shared_ptr<DepthStencilView>>& depthStencilViews);
		void	setViewports(const std::vector<std::shared_ptr<D3D11_VIEWPORT>>& viewPorts);
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	private:
		void mapRenderTargetViews(std::vector<ID3D11RenderTargetView*>& out, const std::vector<std::shared_ptr<RenderTargetView>>& in);
		void mapDepthStencilViews(std::vector<ID3D11DepthStencilView*>& out, const std::vector<std::shared_ptr<DepthStencilView>>& in);
		void mapViewPorts(std::vector<D3D11_VIEWPORT*>& out, const std::vector<std::shared_ptr<D3D11_VIEWPORT>>& in);
		
	private:
		std::vector<std::shared_ptr<RenderTargetView>>	m_renderTargetViews;
		std::vector<std::shared_ptr<DepthStencilView>>	m_depthStencilViews;
		std::vector<std::shared_ptr<D3D11_VIEWPORT>>	m_viewPorts;
	};
}