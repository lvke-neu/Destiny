#pragma once
#include <memory>
#include <vector>
#include <unordered_set>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11Texture2D;
namespace Destiny
{
	class RenderParameters;
	class GraphicsPipeline;
	class GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();
	public:
		void initialize(long long hwnd);
		void uninitialize();
		void update();
		void commitRenderParameters(std::shared_ptr<RenderParameters> renderParameters);
		void commitRenderParameters(const std::unordered_set<std::shared_ptr<RenderParameters>>& renderParameters);
		std::shared_ptr<GraphicsPipeline> getForwardOpaquePipeline();
	public:
		ID3D11Device*				getDevice();
		ID3D11DeviceContext*		getImmediateContext();
		ID3D11DeviceContext*		getDeferredContext();
		//swapchain
		ID3D11RenderTargetView**	getRenderTargetView();
		//swapchain
		ID3D11DepthStencilView*		getDepthStencilView();
	public:
		//for window resize
		void onResize_(unsigned int width, unsigned int height);
	private:
		void createDeviceAndContext();
		void createSwapChain(long long hwnd);
		void createPipeline();
		void render();
	private:
		ID3D11Device*												m_pD3D11Device;
		ID3D11DeviceContext*										m_pD3D11ImmediateDeviceContext;
		ID3D11DeviceContext*										m_pD3D11DeferredDeviceContext;
		IDXGISwapChain*												m_pDXGISwapChain;
		ID3D11RenderTargetView*										m_pRenderTargetView;
		ID3D11Texture2D*											m_pDepthStencilBuffer;
		ID3D11DepthStencilView*										m_pDepthStencilView;
		unsigned int												m_4xMsaaQuality;
		std::unordered_set<std::shared_ptr<RenderParameters>>		m_renderParameters;
		std::shared_ptr<GraphicsPipeline>							m_forwardOpaquePipeline;
	};

	inline ID3D11Device* GraphicsSystem::getDevice()
	{
		return m_pD3D11Device;
	}

	inline ID3D11DeviceContext* GraphicsSystem::getImmediateContext()
	{
		return m_pD3D11ImmediateDeviceContext;
	}

	inline ID3D11DeviceContext* GraphicsSystem::getDeferredContext()
	{
		return m_pD3D11DeferredDeviceContext;
	}

	inline ID3D11RenderTargetView** GraphicsSystem::getRenderTargetView()
	{
		return &m_pRenderTargetView;
	}

	inline ID3D11DepthStencilView* GraphicsSystem::getDepthStencilView()
	{
		return m_pDepthStencilView;
	}

	inline std::shared_ptr<GraphicsPipeline> GraphicsSystem::getForwardOpaquePipeline()
	{
		return m_forwardOpaquePipeline;
	}
}