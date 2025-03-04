#include "BindRenderTargets.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include <d3d11.h>

namespace Destiny
{
	BindRenderTargets::BindRenderTargets() 
	{

	}

	BindRenderTargets::~BindRenderTargets()
	{
		
	}

	std::shared_ptr<RenderTargetView> BindRenderTargets::getRenderTargetViews(unsigned int index)
	{
		if (index < 0 || index>m_renderTargetViews.size() - 1)
		{
			return nullptr;
		}
		return m_renderTargetViews[index];
	}

	std::shared_ptr<DepthStencilView> BindRenderTargets::getDepthStencilViews(unsigned int index)
	{
		if (index < 0 || index>m_renderTargetViews.size() - 1)
		{
			return nullptr;
		}
		return m_depthStencilViews[index];
	}

	void BindRenderTargets::setRenderTargetViews(const std::vector<std::shared_ptr<RenderTargetView>>& renderTargetViews)
	{
		m_renderTargetViews = renderTargetViews;
	}

	void BindRenderTargets::setDepthStencilViews(const std::vector<std::shared_ptr<DepthStencilView>>& depthStencilViews)
	{
		m_depthStencilViews = depthStencilViews;
	}

	void BindRenderTargets::setViewports(const std::vector<std::shared_ptr<D3D11_VIEWPORT>>& viewPorts)
	{
		m_viewPorts = viewPorts;
	}

	void BindRenderTargets::execute(ID3D11DeviceContext* deviceContext)
	{
		if (!deviceContext || m_renderTargetViews.empty() || m_depthStencilViews.empty() || m_viewPorts.empty())
		{
			return;
		}

		std::vector<ID3D11RenderTargetView*> renderTargetViews;
		std::vector<ID3D11DepthStencilView*> depthStencilViews;
		std::vector<D3D11_VIEWPORT*> viewPorts;

		mapRenderTargetViews(renderTargetViews, m_renderTargetViews);
		mapDepthStencilViews(depthStencilViews, m_depthStencilViews);
		mapViewPorts(viewPorts, m_viewPorts);

		deviceContext->RSSetViewports((unsigned int)viewPorts.size(), viewPorts[0]);
		if (renderTargetViews.size() == depthStencilViews.size())
		{
			deviceContext->OMSetRenderTargets((unsigned int)renderTargetViews.size(), renderTargetViews.data(), depthStencilViews[0]);
		}
	}

	void BindRenderTargets::mapRenderTargetViews(std::vector<ID3D11RenderTargetView*>& out, const std::vector<std::shared_ptr<RenderTargetView>>& in)
	{
		out.clear();
		for (const auto& renderTargetView : in)
		{
			if (renderTargetView)
			{
				out.push_back(*renderTargetView->getRenderTargetView());
			}
		}
	}

	void BindRenderTargets::mapDepthStencilViews(std::vector<ID3D11DepthStencilView*>& out, const std::vector<std::shared_ptr<DepthStencilView>>& in)
	{
		out.clear();
		for (const auto& depthStencilView : in)
		{
			if (depthStencilView)
			{
				out.push_back(depthStencilView->getDepthStencilView());
			}
		}
	}

	void BindRenderTargets::mapViewPorts(std::vector<D3D11_VIEWPORT*>& out, const std::vector<std::shared_ptr<D3D11_VIEWPORT>>& in)
	{
		out.clear();
		for (const auto& viewPort : in)
		{
			if (viewPort)
			{
				out.push_back(viewPort.get());
			}
		}
	}
}