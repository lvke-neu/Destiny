#define NOMINMAX
#include "GraphicsSystem.h"
#include "Engine/Utility.h"
#include "Math/Color.h"
#include <d3d11.h>

namespace Destiny
{
	GraphicsSystem::GraphicsSystem() : 
		m_pD3D11Device(nullptr), 
		m_pD3D11ImmediateDeviceContext(nullptr),
		m_pD3D11DeferredDeviceContext(nullptr),
		m_pDXGISwapChain(nullptr),
		m_pRenderTargetView(nullptr),
		m_pDepthStencilBuffer(nullptr),
		m_pDepthStencilView(nullptr),
		m_4xMsaaQuality(0),
		m_viewPort(std::make_shared<D3D11_VIEWPORT>())
	{

	}

	GraphicsSystem::~GraphicsSystem()
	{
		m_pD3D11ImmediateDeviceContext->ClearState();
		SAFE_RELEASE(m_pD3D11Device);
		SAFE_RELEASE(m_pD3D11ImmediateDeviceContext);
		SAFE_RELEASE(m_pD3D11DeferredDeviceContext);
		SAFE_RELEASE(m_pDXGISwapChain);
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pDepthStencilBuffer);
		SAFE_RELEASE(m_pDepthStencilView);
	}

	void GraphicsSystem::initialize(long long hwnd)
	{
		createDeviceAndContext();
		createSwapChain(hwnd);
		onResize_(0, 0);
		createPipeline();
	}

	void GraphicsSystem::uninitialize()
	{
	}

	void GraphicsSystem::update()
	{
		render();
		bindEditorRenderTarget();
	}

	void GraphicsSystem::onResize_(unsigned int width, unsigned int height)
	{
		if (!width || !height)
		{
			return;
		}

		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pDepthStencilBuffer);
		SAFE_RELEASE(m_pDepthStencilView);

		ID3D11Texture2D* backBuffer{ nullptr };
		m_pDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		if (backBuffer)
		{
			m_pD3D11Device->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTargetView);
		}
		SAFE_RELEASE(backBuffer);

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		m_pD3D11Device->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
		if (m_pDepthStencilBuffer)
		{
			m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
		}

		m_viewPort->TopLeftX = 0.0f;
		m_viewPort->TopLeftY = 0.0f;
		m_viewPort->Width = (float)width;
		m_viewPort->Height = (float)height;
		m_viewPort->MinDepth = 0.0f;
		m_viewPort->MaxDepth = 1.0f;
	}

	void GraphicsSystem::createDeviceAndContext()
	{
		HRESULT hr = S_OK;

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0
		};
		D3D_FEATURE_LEVEL featureLevel;
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, &m_pD3D11Device, &featureLevel, &m_pD3D11ImmediateDeviceContext);

		if (FAILED(hr))
		{
			LOG_ERROR("D3D11CreateDevice Failed.");
			return;
		}

		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			LOG_ERROR("Direct3D Feature Level 11_0 unsupported.");
			return;
		}
		m_pD3D11Device->CreateDeferredContext(0, &m_pD3D11DeferredDeviceContext);
	}

	void GraphicsSystem::createSwapChain(long long hwnd)
	{
		m_pD3D11Device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);

		IDXGIDevice* dxgiDevice{ nullptr };
		IDXGIAdapter* dxgiAdapter{ nullptr };
		IDXGIFactory* dxgiFactory{ nullptr };

		m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = (HWND)hwnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		dxgiFactory->CreateSwapChain(m_pD3D11Device, &sd, &m_pDXGISwapChain);

		SAFE_RELEASE(dxgiDevice);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_RELEASE(dxgiFactory);
	}

	void GraphicsSystem::bindEditorRenderTarget()
	{
		m_pD3D11ImmediateDeviceContext->RSSetViewports(1, m_viewPort.get());
		m_pD3D11ImmediateDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		static Color color{ 0, 0, 0, 255 };
		m_pD3D11ImmediateDeviceContext->ClearRenderTargetView(m_pRenderTargetView, (float*)&color);
		m_pD3D11ImmediateDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void GraphicsSystem::present()
	{
		m_pDXGISwapChain->Present(0, 0);
	}
}