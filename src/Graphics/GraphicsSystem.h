#pragma once
#include <memory>
#include <d3d11.h>

namespace Destiny
{
	class Blob;
	class IndexBuffer;
	class GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();
	public:
		void initialize(long long hwnd);
		void uninitialize();
		void draw();
		
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getImmediateContext();
		ID3D11DeviceContext* getDeferredContext();
		ID3D11RenderTargetView* getRenderTargetView();
	public:
		std::shared_ptr<IndexBuffer> createIndexBuffer(DXGI_FORMAT format, std::shared_ptr<Blob> indices);
	private:
		void createDeviceAndContext();
		void createSwapChain(long long hwnd);
		void onResize(void* data);
		void onResize_(unsigned int width, unsigned int height);
	private:
		ID3D11Device* m_pD3D11Device;
		ID3D11DeviceContext* m_pD3D11ImmediateDeviceContext;
		ID3D11DeviceContext* m_pD3D11DeferredDeviceContext;
		IDXGISwapChain* m_pDXGISwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;
		unsigned int m_4xMsaaQuality;
		D3D11_VIEWPORT* m_viewport;
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

	inline ID3D11RenderTargetView* GraphicsSystem::getRenderTargetView()
	{
		return m_pRenderTargetView;
	}
}