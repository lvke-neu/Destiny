#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
namespace Destiny
{
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
	private:
		void createDeviceAndContext();
		void createSwapChain(long long hwnd);
	private:
		ID3D11Device* m_pD3D11Device;
		ID3D11DeviceContext* m_pD3D11DeviceContext;
		IDXGISwapChain* m_pDXGISwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;
		unsigned int m_4xMsaaQuality;
	};
}