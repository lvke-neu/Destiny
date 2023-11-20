#include "GraphicsSystem.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Engine/Utility.h"
#include "Engine/Blob.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "SamplerState.h"
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
		m_viewport(new D3D11_VIEWPORT)
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
		SAFE_DELETE(m_viewport);
	}

	void GraphicsSystem::initialize(long long hwnd)
	{
		createDeviceAndContext();
		createSwapChain(hwnd);
		onResize_(0, 0);

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&GraphicsSystem::onResize, this, std::placeholders::_1));


	}

	void GraphicsSystem::uninitialize()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&GraphicsSystem::onResize, this, std::placeholders::_1));
	}

	void GraphicsSystem::begin()
	{
		static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pD3D11ImmediateDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
		m_pD3D11ImmediateDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void GraphicsSystem::end()
	{
		m_pDXGISwapChain->Present(0, 0);
	}
	
	void GraphicsSystem::addCommandList(ID3D11CommandList* commandList)
	{
		//if (!commandList)
		//{
		//	return;
		//}
		//auto iter = std::find(m_commandLists.begin(), m_commandLists.end(), commandList);
		//if (iter == m_commandLists.end())
		//{
		//	commandList->AddRef();
		//	m_commandLists.push_back(commandList);
		//}
	}

	std::shared_ptr<VertexBuffer> GraphicsSystem::createVertexBuffer(unsigned int stride, unsigned int offset, std::shared_ptr<Blob> vertexData)
	{
		std::shared_ptr<Blob> data = std::make_shared<Blob>(sizeof(unsigned int) * 2 + vertexData->getLength());
		memcpy_s(data->getData(), sizeof(unsigned int), &stride, sizeof(unsigned int));
		memcpy_s((char*)data->getData() + sizeof(unsigned int), sizeof(unsigned int), &offset, sizeof(unsigned int));
		memcpy_s((char*)data->getData() + sizeof(unsigned int) * 2, vertexData->getLength(), vertexData->getData(), vertexData->getLength());

		std::shared_ptr<BlobHolder> blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(data);

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
		vertexBuffer->initialize(nullptr, blobHolder);

		return vertexBuffer;
	}

	std::shared_ptr<IndexBuffer> GraphicsSystem::createIndexBuffer(DXGI_FORMAT format, std::shared_ptr<Blob> indexData)
	{
		std::shared_ptr<Blob> data = std::make_shared<Blob>(sizeof(DXGI_FORMAT) + indexData->getLength());
		memcpy_s(data->getData(), sizeof(DXGI_FORMAT), &format, sizeof(DXGI_FORMAT));
		memcpy_s((char*)data->getData() + sizeof(DXGI_FORMAT), indexData->getLength(), indexData->getData(), indexData->getLength());

		std::shared_ptr<BlobHolder> blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(data);

		std::shared_ptr<IndexBuffer> indexbuffer = std::make_shared<IndexBuffer>();
		indexbuffer->initialize(nullptr, blobHolder);

		return indexbuffer;
	}

	std::shared_ptr<Mesh> GraphicsSystem::createMesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertexBuffer, indexBuffer);
		return mesh;
	}

	std::shared_ptr<VertexShader> GraphicsSystem::createVertexShader(const char* path)
	{
		std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>();

		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			auto blobHolder = blobLoader->createBlobHolder(path);
			vertexShader->initialize(nullptr, blobHolder);
		}
		
		return vertexShader;
	}

	std::shared_ptr<PixelShader> GraphicsSystem::createPixelShader(const char* path)
	{
		std::shared_ptr<PixelShader> pixelShader = std::make_shared<PixelShader>();

		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			auto blobHolder = blobLoader->createBlobHolder(path);
			pixelShader->initialize(nullptr, blobHolder);
		}
		
		return pixelShader;
	}

	std::shared_ptr<InputLayout> GraphicsSystem::createInputLayout(std::shared_ptr<Blob> inputElements, const char* vsPath)
	{
		std::shared_ptr<InputLayout> inputLayout = std::make_shared<InputLayout>(inputElements);
		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(vsPath);
		if (blobLoader)
		{
			auto blobHolder = blobLoader->createBlobHolder(vsPath);
			inputLayout->initialize(nullptr, blobHolder);
		}

		return inputLayout;
	}

	std::shared_ptr<RasterizerState> GraphicsSystem::createRasterizerState(std::shared_ptr<Blob> rasterizerDesc)
	{
		std::shared_ptr<RasterizerState> rasterizerState = std::make_shared<RasterizerState>();

		std::shared_ptr<BlobHolder> blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(rasterizerDesc);
		rasterizerState->initialize(nullptr, blobHolder);

		return rasterizerState;
	}

	std::shared_ptr<DepthStencilState> GraphicsSystem::createDepthStencilState(std::shared_ptr<Blob> depthStencilStateDesc)
	{
		std::shared_ptr<DepthStencilState> depthStencilState = std::make_shared<DepthStencilState>();

		std::shared_ptr<BlobHolder> blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(depthStencilStateDesc);
		depthStencilState->initialize(nullptr, blobHolder);

		return depthStencilState;
	}

	std::shared_ptr<BlendState> GraphicsSystem::createBlendState(std::shared_ptr<Blob> blendStateDesc)
	{
		std::shared_ptr<BlendState> blendState = std::make_shared<BlendState>();

		std::shared_ptr<BlobHolder> blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(blendStateDesc);
		blendState->initialize(nullptr, blobHolder);

		return blendState;
	}

	std::shared_ptr<SamplerState> GraphicsSystem::createSamplerState(std::shared_ptr<Blob> samplerStateDesc)
	{
		std::shared_ptr<SamplerState> samplerState = std::make_shared<SamplerState>();

		std::shared_ptr<BlobHolder> blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(samplerStateDesc);
		samplerState->initialize(nullptr, blobHolder);

		return samplerState;
	}

	void GraphicsSystem::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		onResize_(wrd.width, wrd.height);
	}

	void GraphicsSystem::onResize_(unsigned int width, unsigned int height)
	{
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pDepthStencilBuffer);
		SAFE_RELEASE(m_pDepthStencilView);

		ID3D11Texture2D* backBuffer{ nullptr };
		m_pDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		m_pD3D11Device->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTargetView);
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
		m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);

		m_pD3D11ImmediateDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		//m_pD3D11DeferredDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		
		m_viewport->TopLeftX = 0;
		m_viewport->TopLeftY = 0;
		m_viewport->Width = static_cast<float>(width);
		m_viewport->Height = static_cast<float>(height);
		m_viewport->MinDepth = 0.0f;
		m_viewport->MaxDepth = 1.0f;

		m_pD3D11ImmediateDeviceContext->RSSetViewports(1, m_viewport);
		//m_pD3D11DeferredDeviceContext->RSSetViewports(1, m_viewport);
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
}