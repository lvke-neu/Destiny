#pragma once
#include <memory>
#include <vector>
#include <d3d11.h>

namespace Destiny
{
	class Blob;
	class VertexBuffer;
	class IndexBuffer;
	class Mesh;
	class VertexShader;
	class PixelShader;
	class GeometryShader;
	class InputLayout;
	class RasterizerState;
	class DepthStencilState;
	class BlendState;
	class SamplerState;
	class Visual3D;
	class Texture;
	class TextureLoader;
	class Material;
	class MaterialLoader;
	class RenderTargetView;
	class DepthStencilView;
	class GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();
	public:
		void initialize(long long hwnd);
		void uninitialize();
	public:
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getImmediateContext();
		ID3D11DeviceContext* getDeferredContext();
		ID3D11RenderTargetView** getRenderTargetView();
		ID3D11DepthStencilView* getDepthStencilView();
		D3D11_VIEWPORT* getViewport();
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

	inline ID3D11RenderTargetView** GraphicsSystem::getRenderTargetView()
	{
		return &m_pRenderTargetView;
	}

	inline ID3D11DepthStencilView* GraphicsSystem::getDepthStencilView()
	{
		return m_pDepthStencilView;
	}

	inline D3D11_VIEWPORT* GraphicsSystem::getViewport()
	{
		return m_viewport;
	}
}