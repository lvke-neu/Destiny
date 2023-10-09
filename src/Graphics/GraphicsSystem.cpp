#include "GraphicsSystem.h"
#include "Engine/Utility.h"
#include <d3d11.h>

namespace Destiny
{
	GraphicsSystem::GraphicsSystem() : 
		m_pD3D11Device(nullptr), 
		m_pD3D11DeviceContext(nullptr),
		m_4xMsaaQuality(0)
	{

	}

	GraphicsSystem::~GraphicsSystem()
	{
		SAFE_RELEASE(m_pD3D11Device);
		SAFE_RELEASE(m_pD3D11DeviceContext);
	}

	void GraphicsSystem::initialize()
	{
		HRESULT hr = S_OK;

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0
		};
		D3D_FEATURE_LEVEL featureLevel;
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, &m_pD3D11Device, &featureLevel, &m_pD3D11DeviceContext);

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

		m_pD3D11Device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);

		IDXGIDevice* dxgiDevice = nullptr;
		IDXGIAdapter* dxgiAdapter = nullptr;
		IDXGIFactory* dxgiFactory = nullptr;
		
		m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

		//DXGI_SWAP_CHAIN_DESC sd;
		//ZeroMemory(&sd, sizeof(sd));
		//sd.BufferDesc.Width = m_ClientWidth;
		//sd.BufferDesc.Height = m_ClientHeight;
		//sd.BufferDesc.RefreshRate.Numerator = 60;
		//sd.BufferDesc.RefreshRate.Denominator = 1;
		//sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	}

	void GraphicsSystem::uninitialize()
	{

	}
}