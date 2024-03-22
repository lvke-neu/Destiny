#include "Renderer.h"
#include "Engine/Engine.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/GraphicsSystem.h"
#include <d3d11.h>
#include <d3dcompiler.h>

namespace Destiny
{
	Renderer::Renderer(const char* path) :
		m_vertexShader(nullptr),
		m_pixelShader(nullptr),
		m_blobHolder(nullptr)
	{
		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			m_blobHolder = blobLoader->createBlobHolder(path);
			m_blobHolder->load(0);
		}
	}

	Renderer::~Renderer()
	{
		m_blobHolder.reset();
		SAFE_RELEASE(m_vertexShader);
		SAFE_RELEASE(m_pixelShader);
	}

	void Renderer::doLoad()
	{
		if (!m_blobHolder)
		{
			loadFailed__();
			return;
		}

		if (createVertexShader() && createPixelShader())
		{
			loadSucceeded__();
			m_blobHolder.reset();
		}
		else
		{
			loadFailed__();
			m_blobHolder.reset();
		}	
	}

	bool Renderer::createVertexShader()
	{
		ID3DBlob* ppBlobOut = nullptr;
		ID3DBlob* errorBlob = nullptr;

		HRESULT hr = D3DCompileFromFile(Utility::MultiByte2WideChar(m_blobHolder->getFullPath()).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &ppBlobOut, &errorBlob);

		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CompileVertexShader {0} Failed:{1}", m_blobHolder->getFullPath(), reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CompileVertexShader {0} Failed:{1}", m_blobHolder->getFullPath(), "path error");
			}
			SAFE_RELEASE(errorBlob);
			return false;
		}
		
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateVertexShader(ppBlobOut->GetBufferPointer(), ppBlobOut->GetBufferSize(), 0, &m_vertexShader);
		if (FAILED(hr))
		{
			LOG_ERROR("CreateVertexShader {0} failed", m_blobHolder->getFullPath());
			SAFE_RELEASE(ppBlobOut);
			return false;
		}

		SAFE_RELEASE(ppBlobOut);
		return true;
	}

	bool Renderer::createPixelShader()
	{
		ID3DBlob* ppBlobOut = nullptr;
		ID3DBlob* errorBlob = nullptr;

		HRESULT hr = D3DCompileFromFile(Utility::MultiByte2WideChar(m_blobHolder->getFullPath()).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &ppBlobOut, &errorBlob);

		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CreatePixelShader {0} Failed:{1}", m_blobHolder->getFullPath(), reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CreatePixelShader {0} Failed:{1}", m_blobHolder->getFullPath(), "path error");
			}
			SAFE_RELEASE(errorBlob);
			return false;
		}

		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreatePixelShader(ppBlobOut->GetBufferPointer(), ppBlobOut->GetBufferSize(), 0, &m_pixelShader);
		if (FAILED(hr))
		{
			LOG_ERROR("CreatePixelShader {0} failed", m_blobHolder->getFullPath());
			SAFE_RELEASE(ppBlobOut);
			return false;
		}

		SAFE_RELEASE(ppBlobOut);
		return true;
	}
}