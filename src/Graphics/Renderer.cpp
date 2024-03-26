#include "Renderer.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
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
		SAFE_RELEASE(m_inputSignatureBlob);
	}

	void Renderer::doLoad()
	{
		if (!m_blobHolder || !m_blobHolder->isLoadingSucceed() || !m_blobHolder->getBlob())
		{
			loadFailed__();
			return;
		}

		(createVertexShader() && createPixelShader()) ? loadSucceeded__() : loadFailed__();
	}

	bool Renderer::createVertexShader()
	{
		ID3DBlob* ppBlobOut = nullptr;
		ID3DBlob* errorBlob = nullptr;

		auto blob = m_blobHolder->getBlob();
		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), m_blobHolder->getFullPath().c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &ppBlobOut, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CompileShader Failed:{0}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CompileShader {0} Failed:{1}", m_blobHolder->getFullPath(), "path error");
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

		D3DGetInputSignatureBlob(ppBlobOut->GetBufferPointer(), ppBlobOut->GetBufferSize(), &m_inputSignatureBlob);
		SAFE_RELEASE(ppBlobOut);
		return true;
	}

	bool Renderer::createPixelShader()
	{
		ID3DBlob* ppBlobOut = nullptr;
		ID3DBlob* errorBlob = nullptr;

		auto blob = m_blobHolder->getBlob();
		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), m_blobHolder->getFullPath().c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &ppBlobOut, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CreateShader Failed:{0}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CreateShader {0} Failed:{1}", m_blobHolder->getFullPath(), "path error");
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