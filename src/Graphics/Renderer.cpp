#include "Renderer.h"
#include "Texture.h"
#include "SamplerState.h"
#include "GraphicsSystem.h"
#include "DrawParameters.h"
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
		m_geometryShader(nullptr),
		m_inputSignatureBlob(nullptr),
		m_blobHolder(nullptr),
		m_vsCompiledBlob(nullptr),
		m_psCompiledBlob(nullptr),
		m_gsCompiledBlob(nullptr)
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
		SAFE_RELEASE(m_gsCompiledBlob);
	}

	void Renderer::doLoad()
	{
		if (isLoadingSucceed())
		{
			return;
		}

		if (!m_blobHolder || !m_blobHolder->isLoadingSucceed() || !m_blobHolder->getBlob() || !m_blobHolder->getBlobLoader())
		{
			loadFailed__();
			m_inputSignatureBlob.reset();
			m_blobHolder.reset();
			return;
		}

		(createVertexShader() && createPixelShader() && createGeometryShader()) ? loadSucceeded__() : loadFailed__();
		

		//m_blobHolder.reset();

		collectReflectionInfo(m_vsCompiledBlob, 0);
		collectReflectionInfo(m_psCompiledBlob, 1);
		collectReflectionInfo(m_gsCompiledBlob, 2);
		SAFE_RELEASE(m_vsCompiledBlob);
		SAFE_RELEASE(m_psCompiledBlob);
		SAFE_RELEASE(m_gsCompiledBlob);
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

	std::string Renderer::getPath() 
	{
		if (m_blobHolder)
		{
			return m_blobHolder->getPath();
		}
		return "";
	}

	bool Renderer::createVertexShader()
	{
		ID3DBlob* errorBlob = nullptr;

		auto blob = m_blobHolder->getBlob();
		auto normalizedPath = m_blobHolder->getBlobLoader()->normalizedPath(m_blobHolder);

		unsigned int flag = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		flag |= D3DCOMPILE_DEBUG;
		flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // 

		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), normalizedPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
			flag, 0, &m_vsCompiledBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CompileVertexShader Failed:{0}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CompileVertexShader {0} Failed:{1}", normalizedPath, "path error");
			}
			SAFE_RELEASE(errorBlob);
			return false;
		}
		
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateVertexShader(m_vsCompiledBlob->GetBufferPointer(), m_vsCompiledBlob->GetBufferSize(), 0, &m_vertexShader);
		if (FAILED(hr))
		{
			
			LOG_ERROR("CreateVertexShader {0} failed", normalizedPath);
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
		auto normalizedPath = m_blobHolder->getBlobLoader()->normalizedPath(m_blobHolder);

		unsigned int flag = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		flag |= D3DCOMPILE_DEBUG;
		flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // 

		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), normalizedPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
			flag, 0, &m_psCompiledBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CompilePixelShader Failed:{0}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CompilePixelShader {0} Failed:{1}", normalizedPath, "path error");
			}
			SAFE_RELEASE(errorBlob);
			return false;
		}
		
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreatePixelShader(m_psCompiledBlob->GetBufferPointer(), m_psCompiledBlob->GetBufferSize(), 0, &m_pixelShader);
		if (FAILED(hr))
		{
			
			LOG_ERROR("CreatePixelShader {0} failed", normalizedPath);
			return false;
		}
		
		return true;
	}

	bool Renderer::createGeometryShader()
	{
		ID3DBlob* errorBlob = nullptr;

		auto blob = m_blobHolder->getBlob();

		std::string content((char*)blob->getData(), blob->getLength());
		if (content.find("GS") == content.npos)
		{
			return true;
		}

		auto normalizedPath = m_blobHolder->getBlobLoader()->normalizedPath(m_blobHolder);

		unsigned int flag = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		flag |= D3DCOMPILE_DEBUG;
		flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // 

		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), normalizedPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "gs_5_0",
			flag, 0, &m_gsCompiledBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CompileGeometryShader Failed:{0}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CompileGeometryShader {0} Failed:{1}", normalizedPath, "path error");
			}
			SAFE_RELEASE(errorBlob);
			return false;
		}
		
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateGeometryShader(m_gsCompiledBlob->GetBufferPointer(), m_gsCompiledBlob->GetBufferSize(), 0, &m_geometryShader);
		if (FAILED(hr))
		{
			
			LOG_ERROR("CreateGeometryShader {0} failed", normalizedPath);
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

	void Renderer::fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters)
	{
		if (!drawParameters)
		{
			return;
		}

		drawParameters->vertexShader = m_vertexShader;
		drawParameters->pixelShader = m_pixelShader;
		drawParameters->geometryShader = m_geometryShader;
		drawParameters->constantBuffers = m_constantBuffers;
		drawParameters->textures = m_textures;
		drawParameters->samplerStates = m_samplerStates;
	}

	void Renderer::copy_constant_texture_sampler(std::shared_ptr<Renderer> other)
	{
		if (!other)
		{
			return;
		}

		for (const auto& constant : other->m_constantBuffers)
		{
			m_constantBuffers.insert(constant);
		}

		for (const auto& link : other->m_variableLinkConstant)
		{
			m_variableLinkConstant.insert(link);
		}

		for (const auto& texture : other->m_textures)
		{
			m_textures.insert(texture);
		}
		
		for (const auto& sampler : other->m_samplerStates)
		{
			m_samplerStates.insert(sampler);
		}
	}
}