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
		void begin();
		void end();
		
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getImmediateContext();
		ID3D11DeviceContext* getDeferredContext();
		ID3D11RenderTargetView** getRenderTargetView();
		ID3D11DepthStencilView* getDepthStencilView();
		D3D11_VIEWPORT* getViewport();
		std::shared_ptr<RenderTargetView> getRenderToShadowMapRTV();
		std::shared_ptr<DepthStencilView> getRenderToShadowMapDSV();
		std::shared_ptr<VertexShader> getShadowMapVertexShader();
		std::shared_ptr<SamplerState> getShadowMapSamplerState();
	public:
		void commitVisual3D(std::shared_ptr<Visual3D> visual3D);
	public:
		std::shared_ptr<VertexBuffer> createVertexBuffer(unsigned int stride, unsigned int offset, std::shared_ptr<Blob> vertexData);
		std::shared_ptr<IndexBuffer> createIndexBuffer(DXGI_FORMAT format, std::shared_ptr<Blob> indexData);
		std::shared_ptr<Mesh> createMesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);
		std::shared_ptr<VertexShader> createVertexShader(const char* path);
		std::shared_ptr<PixelShader> createPixelShader(const char* path);
		std::shared_ptr<PixelShader> compilePixelShader(const char* path);
		std::shared_ptr<GeometryShader> createGeometryShader(const char* path);
		std::shared_ptr<GeometryShader> compileGeometryShader(const char* path);
		std::shared_ptr<InputLayout> createInputLayout(std::shared_ptr<Blob> inputElements, const char* vsPath);
		std::shared_ptr<RasterizerState> createRasterizerState(std::shared_ptr<Blob> rasterizerDesc);
		std::shared_ptr<DepthStencilState> createDepthStencilState(std::shared_ptr<Blob> depthStencilStateDesc);
		std::shared_ptr<BlendState> createBlendState(std::shared_ptr<Blob> blendStateDesc);
		std::shared_ptr<SamplerState> createSamplerState(std::shared_ptr<Blob> samplerStateDesc);
		std::shared_ptr<Texture> createTexture(const char* path);
		std::shared_ptr<Material> createMaterial();
		
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
		std::vector<std::shared_ptr<Visual3D>> m_visual3Ds;
		std::shared_ptr<TextureLoader> m_textureLoader;
		std::shared_ptr<MaterialLoader> m_materialLoader;

		std::shared_ptr<RenderTargetView> m_renderToShadowMapRTV;
		std::shared_ptr<DepthStencilView> m_renderToShadowMapDSV;
		std::shared_ptr<VertexShader> m_shadowMapVertexShader;
		std::shared_ptr<SamplerState> m_shadowMapSamplerState;
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

	inline std::shared_ptr<RenderTargetView> GraphicsSystem::getRenderToShadowMapRTV()
	{
		return m_renderToShadowMapRTV;
	}

	inline std::shared_ptr<DepthStencilView> GraphicsSystem::getRenderToShadowMapDSV()
	{
		return m_renderToShadowMapDSV;
	}

	inline std::shared_ptr<VertexShader> GraphicsSystem::getShadowMapVertexShader()
	{
		return m_shadowMapVertexShader;
	}

	inline std::shared_ptr<SamplerState> GraphicsSystem::getShadowMapSamplerState()
	{
		return m_shadowMapSamplerState;
	}
}