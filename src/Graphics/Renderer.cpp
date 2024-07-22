#include "Renderer.h"
#include "Texture.h"
#include "SamplerState.h"
#include "RenderStates.h"

#include "GraphicsSystem.h"
#include "RenderParameters.h"
#include "Engine/Engine.h"

#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobLoaderManager.h"
#include <d3d11.h>
#include <d3dcompiler.h>

namespace Destiny
{
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
		if (isLoadingSucceed())
		{
			return;
		}

		if (!m_blobHolder || !m_blobHolder->isLoadingSucceed() || !m_blobHolder->getBlob())
		{
			loadFailed__();
			m_inputSignatureBlob.reset();
			m_blobHolder.reset();
			return;
		}

		(createVertexShader() && createPixelShader()) ? loadSucceeded__() : loadFailed__();
		m_blobHolder.reset();

		collectReflectionInfo(m_vsCompiledBlob, 0);
		collectReflectionInfo(m_psCompiledBlob, 1);
		SAFE_RELEASE(m_vsCompiledBlob);
		SAFE_RELEASE(m_psCompiledBlob);
	}

	void Renderer::setShaderResource(const char* name, std::shared_ptr<Texture> texture)
	{
		auto iter = m_textures.find(name);
		if (iter == m_textures.end())
		{
			return;
		}

		m_textures[name].second.reset();
		m_textures[name].second = texture;
	}

	void Renderer::setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState)
	{
		auto iter = m_samplerStates.find(name);
		if (iter == m_samplerStates.end())
		{
			return;
		}

		m_samplerStates[name].second.reset();
		m_samplerStates[name].second = samplerState;
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

	void Renderer::collectReflectionInfo(ID3D10Blob* compiledBlob, short flag)
	{
		if (!compiledBlob)
		{
			return;
		}

		HRESULT hr = 0;
		ID3D11ShaderReflection* shaderReflection = nullptr;
		hr = D3DReflect(compiledBlob->GetBufferPointer(), compiledBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)(&shaderReflection));
		if (SUCCEEDED(hr))
		{
			collectReflectionConstantInfo(shaderReflection, flag);
			collectReflectionTextureInfo(shaderReflection, flag);
			collectReflectionSamplerStateInfo(shaderReflection, flag);
		}
	}

	void Renderer::collectReflectionConstantInfo(ID3D11ShaderReflection* shaderReflection, short flag)
	{
		if (!shaderReflection)
		{
			return;
		}

		HRESULT hr = 0;
		D3D11_SHADER_DESC shaderDesc;
		hr = shaderReflection->GetDesc(&shaderDesc);
		if (SUCCEEDED(hr))
		{
			for (unsigned int i = 0; i < shaderDesc.ConstantBuffers; i++)
			{
				ID3D11ShaderReflectionConstantBuffer* reflectionConstantBuffer = nullptr;
				reflectionConstantBuffer = shaderReflection->GetConstantBufferByIndex(i);
				if (reflectionConstantBuffer)
				{
					D3D11_SHADER_BUFFER_DESC shaderBufferDesc;
					hr = reflectionConstantBuffer->GetDesc(&shaderBufferDesc);

					if (SUCCEEDED(hr))
					{
						D3D11_SHADER_INPUT_BIND_DESC shaderInputBindDesc;
						hr = shaderReflection->GetResourceBindingDescByName(shaderBufferDesc.Name, &shaderInputBindDesc);

						if (SUCCEEDED(hr))
						{
							auto iter = m_constantBuffers.find(shaderBufferDesc.Name);
							if (iter == m_constantBuffers.end())
							{
								m_constantBuffers[shaderBufferDesc.Name] = std::make_shared<ConstantBuffer>(shaderInputBindDesc.BindPoint, shaderBufferDesc.Size);
								for (unsigned int j = 0; j < shaderBufferDesc.Variables; j++)
								{
									ID3D11ShaderReflectionVariable* shaderReflectionVariable = reflectionConstantBuffer->GetVariableByIndex(j);
									if (shaderReflectionVariable)
									{
										D3D11_SHADER_VARIABLE_DESC shaderVariableDesc;
										hr = shaderReflectionVariable->GetDesc(&shaderVariableDesc);
										if (SUCCEEDED(hr))
										{
											m_constantBuffers[shaderBufferDesc.Name]->addVariable(shaderVariableDesc.Name, { shaderVariableDesc.StartOffset, shaderVariableDesc.Size });
											m_variableLinkConstant[shaderVariableDesc.Name] = shaderBufferDesc.Name;
										}
									}
								}
							}
							m_constantBuffers[shaderBufferDesc.Name]->setConstantBufferBindFlag((ConstantBufferBindFlag)flag, true);
						}
					}
				}
			}
		}
	}

	void Renderer::collectReflectionTextureInfo(ID3D11ShaderReflection* shaderReflection, short flag)
	{
		if (!shaderReflection)
		{
			return;
		}

		HRESULT hr = 0;
		D3D11_SHADER_DESC shaderDesc;
		hr = shaderReflection->GetDesc(&shaderDesc);
		if (SUCCEEDED(hr))
		{
			for (unsigned int i = 0; ; i++)
			{
				D3D11_SHADER_INPUT_BIND_DESC shaderInputBindDesc;
				hr = shaderReflection->GetResourceBindingDesc(i, &shaderInputBindDesc);
				if (FAILED(hr))
				{
					break;
				}

				if (shaderInputBindDesc.Type == D3D_SIT_TEXTURE)
				{
					auto iter = m_textures.find(shaderInputBindDesc.Name);
					if (iter == m_textures.end())
					{
						m_textures[shaderInputBindDesc.Name] = std::make_pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>> (std::make_shared<TextureDesc>(), nullptr);
					}
					m_textures[shaderInputBindDesc.Name].first->startSlot = shaderInputBindDesc.BindPoint;
					m_textures[shaderInputBindDesc.Name].first->textureBindFlag[(TextureBindFlag)flag] = true;
				}
			}
		}
	}

	void Destiny::Renderer::collectReflectionSamplerStateInfo(ID3D11ShaderReflection* shaderReflection, short flag)
	{
		if (!shaderReflection)
		{
			return;
		}

		HRESULT hr = 0;
		D3D11_SHADER_DESC shaderDesc;
		hr = shaderReflection->GetDesc(&shaderDesc);
		if (SUCCEEDED(hr))
		{
			for (unsigned int i = 0; ; i++)
			{
				D3D11_SHADER_INPUT_BIND_DESC shaderInputBindDesc;
				hr = shaderReflection->GetResourceBindingDesc(i, &shaderInputBindDesc);
				if (FAILED(hr))
				{
					break;
				}

				if (shaderInputBindDesc.Type == D3D_SIT_SAMPLER)
				{
					auto iter = m_samplerStates.find(shaderInputBindDesc.Name);
					if (iter == m_samplerStates.end())
					{
						m_samplerStates[shaderInputBindDesc.Name] = std::make_pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>(std::make_shared<SamplerStateDesc>(), nullptr);
					}
					m_samplerStates[shaderInputBindDesc.Name].first->startSlot = shaderInputBindDesc.BindPoint;
					m_samplerStates[shaderInputBindDesc.Name].first->samplerStateBindFlag[(SamplerStateBindFlag)flag] = true;
				}
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
		renderParameters->constantBuffers = m_constantBuffers;
		renderParameters->textures = m_textures;
		renderParameters->samplerStates = m_samplerStates;
	}
}