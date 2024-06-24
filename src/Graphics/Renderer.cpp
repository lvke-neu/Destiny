#include "Renderer.h"
#include "RenderStates.h"
#include "GraphicsSystem.h"
#include "RenderParameters.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobLoaderManager.h"
#include <d3d11.h>
#include <d3dcompiler.h>

namespace Destiny
{
	#define per_object_constant_buffer_names "cbPerObject"

	Renderer::Renderer(const char* path) :
		m_vertexShader(nullptr),
		m_pixelShader(nullptr),
		m_inputSignatureBlob(nullptr),
		m_blobHolder(nullptr),
		m_vsCompiledBlob(nullptr),
		m_psCompiledBlob(nullptr)
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
		SAFE_RELEASE(m_vertexShader);
		SAFE_RELEASE(m_pixelShader);
		SAFE_RELEASE(m_vsCompiledBlob);
		SAFE_RELEASE(m_psCompiledBlob);
	}

	void Renderer::doLoad()
	{
		if (!m_blobHolder || !m_blobHolder->isLoadingSucceed() || !m_blobHolder->getBlob())
		{
			loadFailed__();
			m_inputSignatureBlob.reset();
			m_blobHolder.reset();
			return;
		}

		(createVertexShader() && createPixelShader()) ? loadSucceeded__() : loadFailed__();
		m_blobHolder.reset();
	}

	bool Renderer::createVertexShader()
	{
		ID3DBlob* errorBlob = nullptr;

		auto blob = m_blobHolder->getBlob();
		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), m_blobHolder->getFullPath().c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_vsCompiledBlob, &errorBlob);
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
		
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateVertexShader(m_vsCompiledBlob->GetBufferPointer(), m_vsCompiledBlob->GetBufferSize(), 0, &m_vertexShader);
		if (FAILED(hr))
		{
			LOG_ERROR("CreateVertexShader {0} failed", m_blobHolder->getFullPath());
			return false;
		}

		ID3D10Blob* inputSignatureBlob = nullptr;

		hr = D3DGetInputSignatureBlob(m_vsCompiledBlob->GetBufferPointer(), m_vsCompiledBlob->GetBufferSize(), &inputSignatureBlob);
		if (SUCCEEDED(hr))
		{
			m_inputSignatureBlob = std::make_shared<Blob>(inputSignatureBlob->GetBufferSize());
			m_inputSignatureBlob->copyfrom(inputSignatureBlob->GetBufferPointer(), inputSignatureBlob->GetBufferSize());
		}

		SAFE_RELEASE(inputSignatureBlob);
		return true;
	}

	bool Renderer::createPixelShader()
	{
		ID3DBlob* errorBlob = nullptr;

		auto blob = m_blobHolder->getBlob();
		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), m_blobHolder->getFullPath().c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_psCompiledBlob, &errorBlob);
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

		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreatePixelShader(m_psCompiledBlob->GetBufferPointer(), m_psCompiledBlob->GetBufferSize(), 0, &m_pixelShader);
		if (FAILED(hr))
		{
			LOG_ERROR("CreatePixelShader {0} failed", m_blobHolder->getFullPath());
			return false;
		}

		return true;
	}

	void Renderer::setFloat(const char* name, float data)
	{
		HRESULT hr = 0;
		ID3D11ShaderReflection* shaderReflection = nullptr;
		if (m_vsCompiledBlob)
		{
			hr = D3DReflect(m_vsCompiledBlob->GetBufferPointer(), m_vsCompiledBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)(&shaderReflection));
			if (SUCCEEDED(hr))
			{
				ID3D11ShaderReflectionConstantBuffer* reflectionConstantBuffer = nullptr;
				reflectionConstantBuffer = shaderReflection->GetConstantBufferByName(per_object_constant_buffer_names);
				if (reflectionConstantBuffer)
				{
					ID3D11ShaderReflectionVariable* reflectionVariable = reflectionConstantBuffer->GetVariableByName(name);
					if (reflectionVariable)
					{
						delete reflectionVariable;
					}
					
					delete reflectionConstantBuffer;
				}
				SAFE_RELEASE(shaderReflection);
			}
		}
	}

	void Renderer::fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters)
	{
		if (!renderParameters)
		{
			return;
		}

		renderParameters->vertexShader = m_vertexShader;
		renderParameters->pixelShader = m_pixelShader;
	}
}