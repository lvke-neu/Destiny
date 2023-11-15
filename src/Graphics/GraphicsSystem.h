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
	class InputLayout;
	class GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();
	public:
		void initialize(long long hwnd);
		void uninitialize();
		void begin();
		void end();
		
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getImmediateContext();
		ID3D11DeviceContext* getDeferredContext();
		ID3D11RenderTargetView** getRenderTargetView();
		ID3D11DepthStencilView* getDepthStencilView();
		D3D11_VIEWPORT* getViewport();
	public:
		void addCommandList(ID3D11CommandList* commandList);
	public:
		std::shared_ptr<VertexBuffer> createVertexBuffer(unsigned int stride, unsigned int offset, std::shared_ptr<Blob> vertexData);
		std::shared_ptr<IndexBuffer> createIndexBuffer(DXGI_FORMAT format, std::shared_ptr<Blob> indexData);
		std::shared_ptr<Mesh> createMesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);
		std::shared_ptr<VertexShader> createVertexShader(const char* path);
		std::shared_ptr<PixelShader> createPixelShader(const char* path);
		std::shared_ptr<InputLayout> createInputLayout(std::shared_ptr<Blob> inputElements, const char* vsPath);
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