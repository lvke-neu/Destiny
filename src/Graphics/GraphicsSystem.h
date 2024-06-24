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
struct D3D11_VIEWPORT;
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
	class RenderParameters;
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
	public:
		ID3D11Device*				getDevice();
		ID3D11DeviceContext*		getImmediateContext();
		ID3D11DeviceContext*		getDeferredContext();
		ID3D11RenderTargetView**	getRenderTargetView();
		ID3D11DepthStencilView*		getDepthStencilView();
	private:
		void createDeviceAndContext();
		void createSwapChain(long long hwnd);
		void onResize(void* data);
		void onResize_(unsigned int width, unsigned int height);
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
		std::shared_ptr<D3D11_VIEWPORT>								m_viewport;
		std::unordered_set<std::shared_ptr<RenderParameters>>		m_renderParameters;
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
}