#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct D3D11_VIEWPORT;
namespace Destiny
{
	class ImagePass;
	class GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();
	public:
		void initialize(long long hwnd, unsigned int width, unsigned int height);
		void uninitialize();
		void draw();
		void onResize(unsigned int width, unsigned int height);
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getImmediateContext();
		ID3D11DeviceContext* getDeferredContext();
		ID3D11RenderTargetView* getRenderTargetView();
	private:
		void createDeviceAndContext();
		void createSwapChain(long long hwnd);
	private:
		ID3D11Device* m_pD3D11Device;
		ID3D11DeviceContext* m_pD3D11DeviceContext;
		ID3D11DeviceContext* m_pD3D11DeferredDeviceContext;
		IDXGISwapChain* m_pDXGISwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;
		unsigned int m_4xMsaaQuality;
		D3D11_VIEWPORT* m_viewport;
		ImagePass* m_imagePass;
	};

	inline ID3D11Device* GraphicsSystem::getDevice()
	{
		return m_pD3D11Device;
	}

	inline ID3D11DeviceContext* GraphicsSystem::getImmediateContext()
	{
		return m_pD3D11DeviceContext;
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