#include "Renderer.h"
#include "Texture.h"
#include "SamplerState.h"
#include "GraphicsSystem.h"
#include "DrawParameters.h"
#include "Engine/Engine.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobLoaderManager.h"
#include "RHI/Vulkan/VulkanDevice.h"
#include "RHI/Vulkan/ShaderCompiler.h"
#include "RHI/Vulkan/VulkanShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>

namespace Destiny
{
	std::unordered_map<std::string, std::shared_ptr<Renderer>> Renderer::s_cache;
	std::unordered_map<std::string, std::shared_ptr<Blob>> Renderer::s_cache_constant;
	std::unordered_map<std::string, std::shared_ptr<Texture>> Renderer::s_cache_shaderResource;
	std::unordered_map<std::string, std::shared_ptr<SamplerState>> Renderer::s_cache_samplerSate;
	Renderer::Renderer(const char* path) :
		m_vertexShader(nullptr),
		m_pixelShader(nullptr),
		m_geometryShader(nullptr),
		m_hullShader(nullptr),
		m_domainShader(nullptr),
		m_inputSignatureBlob(nullptr),
		m_blobHolder(nullptr),
		m_vsCompiledBlob(nullptr),
		m_psCompiledBlob(nullptr),
		m_gsCompiledBlob(nullptr),
		m_hsCompiledBlob(nullptr),
		m_dsCompiledBlob(nullptr)
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
		auto engine = Engine::GetInstance();
		if (engine && engine->getGraphicsSystem())
		{
			auto graphicsSystem = engine->getGraphicsSystem();
			// Check if device is valid (not 0xdddddddd or null)
			// std::shared_ptr handles null checks, but 0xdddddddd implies released memory in debug mode
			// We can try to catch exceptions or rely on engine state
			
			// Safer approach: Only destroy Vulkan objects if we are sure engine is running
			// Or check a static flag?
			
			// For now, let's just check the pointer itself if possible, but 0xdddddddd is a raw pointer pattern
			// shared_ptr shouldn't be 0xdddddddd unless memory corruption or use-after-free of the container
			
			auto device = graphicsSystem->getDevice();
			if (device) // shared_ptr operator bool
			{
				if (std::dynamic_pointer_cast<VulkanDevice>(device))
				{
					if (m_vertexShader) delete (VulkanVertexShader*)m_vertexShader;
					if (m_pixelShader) delete (VulkanPixelShader*)m_pixelShader;
				}
				else
				{
					if (m_vertexShader) ((ID3D11VertexShader*)m_vertexShader)->Release();
					if (m_pixelShader) ((ID3D11PixelShader*)m_pixelShader)->Release();
					if (m_geometryShader) ((ID3D11GeometryShader*)m_geometryShader)->Release();
					if (m_hullShader) ((ID3D11HullShader*)m_hullShader)->Release();
					if (m_domainShader) ((ID3D11DomainShader*)m_domainShader)->Release();
				}
			}
		}
		// If we can't access the device safely, we leak the Vulkan shader objects (CPU side wrappers),
		// but the GPU/Driver resources are tied to the VkDevice which is likely being destroyed anyway.
		// For DX11, we should ideally still Release(), but without a way to know if it's safe...
		
		SAFE_RELEASE(m_vsCompiledBlob);
		SAFE_RELEASE(m_psCompiledBlob);
		SAFE_RELEASE(m_gsCompiledBlob);
		SAFE_RELEASE(m_hsCompiledBlob);
		SAFE_RELEASE(m_dsCompiledBlob);
	}

	void Renderer::doLoad()
	{
		if (isLoadingSucceed() || isLoadingStart())
		{
			return;
		}

		loadStart__();

		if (!m_blobHolder || !m_blobHolder->isLoadingSucceed() || !m_blobHolder->getBlob() || !m_blobHolder->getBlobLoader())
		{
			loadFailed__();
			m_inputSignatureBlob.reset();
			m_blobHolder.reset();
			return;
		}

		(createVertexShader() && createPixelShader() && createGeometryShader() && createHullShader() && createDomainShader()) ? loadSucceeded__() : loadFailed__();
		

		//m_blobHolder.reset();

		collectReflectionInfo(m_vsCompiledBlob, 0);
		collectReflectionInfo(m_psCompiledBlob, 1);
		collectReflectionInfo(m_gsCompiledBlob, 2);
		collectReflectionInfo(m_hsCompiledBlob, 3);
		collectReflectionInfo(m_dsCompiledBlob, 4);

		SAFE_RELEASE(m_vsCompiledBlob);
		SAFE_RELEASE(m_psCompiledBlob);
		SAFE_RELEASE(m_gsCompiledBlob);
		SAFE_RELEASE(m_hsCompiledBlob);
		SAFE_RELEASE(m_dsCompiledBlob);

		for (const auto& constantBlob : s_cache_constant)
		{
			setConstant(constantBlob.first.c_str(), constantBlob.second);
		}

		for (const auto& shaderResource : s_cache_shaderResource)
		{
			setShaderResource(shaderResource.first.c_str(), shaderResource.second);
		}

		for (const auto& samplerSate : s_cache_samplerSate)
		{
			setSamplerSate(samplerSate.first.c_str(), samplerSate.second);
		}
	}

	void Renderer::doReload()
	{
		if (!m_blobHolder)
		{
			return;
		}

		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(m_blobHolder->getPath().c_str());
		if (blobLoader)
		{
			m_blobHolder = blobLoader->createBlobHolder(m_blobHolder->getPath().c_str());
			m_blobHolder->load(0);
		}
		loadPending_();
		doLoad();
	}

	std::shared_ptr<Renderer> Renderer::Create(const char* path)
	{
		auto iter = s_cache.find(path);
		if (iter != s_cache.end())
		{
			return iter->second;
		}

		std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(path);

		s_cache.insert({ path, renderer });
		return renderer;
	}

	void Renderer::SetConstant(const char* name, std::shared_ptr<Blob> blob)
	{
		s_cache_constant[name] = blob;
		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setConstant(name, blob);
		}
	}

	void Renderer::SetShaderResource(const char* name, std::shared_ptr<Texture> texture)
	{
		s_cache_shaderResource[name] = texture;
		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setShaderResource(name, texture);
		}
	}

	void Renderer::SetSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState)
	{
		s_cache_samplerSate[name] = samplerState;
		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setSamplerSate(name, samplerState);
		}
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

		m_changedTextureNames.insert(name);
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
		
		m_changedSamplerStateNames.insert(name);
	}

	void Renderer::ClearCache()
	{
		s_cache.clear();
		s_cache_constant.clear();
		s_cache_shaderResource.clear();
		s_cache_samplerSate.clear();
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

        auto device = Engine::GetInstance()->getGraphicsSystem()->getDevice();
        if (std::dynamic_pointer_cast<VulkanDevice>(device))
        {
            // Vulkan Path
            std::string source((char*)blob->getData(), blob->getLength());
            std::vector<uint32_t> spirv;
            
            // Assume "main" as entry point for now, or extract from somewhere?
            // DX11 path uses "VS".
            if (!ShaderCompiler::CompileToSpirv(normalizedPath, "VS", "vs_6_0", spirv))
            {
                LOG_ERROR("CompileVertexShader (Vulkan) {0} failed", normalizedPath);
                return false;
            }

            HRESULT hr = (HRESULT)device->CreateVertexShader(spirv.data(), spirv.size() * sizeof(uint32_t), nullptr, (void**)&m_vertexShader);
            if (FAILED(hr))
            {
                LOG_ERROR("CreateVertexShader (Vulkan) {0} failed", normalizedPath);
                return false;
            }

            // TODO: Implement SPIR-V Reflection for Input Layout and Constants
            return true;
        }

        // DX11 Path
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
				LOG_ERROR("CompileVertexShader Failed:{0}, {1}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()), normalizedPath);
			}
			else
			{
				LOG_ERROR("CompileVertexShader {0} Failed:{1}", normalizedPath, "path error");
			}
			SAFE_RELEASE(errorBlob);
			return false;
		}
		
		hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateVertexShader(m_vsCompiledBlob->GetBufferPointer(), m_vsCompiledBlob->GetBufferSize(), nullptr, (void**)&m_vertexShader);
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

        auto device = Engine::GetInstance()->getGraphicsSystem()->getDevice();
        if (std::dynamic_pointer_cast<VulkanDevice>(device))
        {
            // Vulkan Path
            std::vector<uint32_t> spirv;
            
            // Assume "PS" as entry point for now
            if (!ShaderCompiler::CompileToSpirv(normalizedPath, "PS", "ps_6_0", spirv))
            {
                LOG_ERROR("CompilePixelShader (Vulkan) {0} failed", normalizedPath);
                return false;
            }

            HRESULT hr = (HRESULT)device->CreatePixelShader(spirv.data(), spirv.size() * sizeof(uint32_t), nullptr, (void**)&m_pixelShader);
            if (FAILED(hr))
            {
                LOG_ERROR("CreatePixelShader (Vulkan) {0} failed", normalizedPath);
                return false;
            }
            return true;
        }

        // DX11 Path
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
		
		hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreatePixelShader(m_psCompiledBlob->GetBufferPointer(), m_psCompiledBlob->GetBufferSize(), nullptr, (void**)&m_pixelShader);
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
		
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateGeometryShader(m_gsCompiledBlob->GetBufferPointer(), m_gsCompiledBlob->GetBufferSize(), 0, (void**)&m_geometryShader);
		if (FAILED(hr))
		{
			
			LOG_ERROR("CreateGeometryShader {0} failed", normalizedPath);
			return false;
		}
		
		return true;
	}

	bool Renderer::createHullShader()
	{
		ID3DBlob* errorBlob = nullptr;

		auto blob = m_blobHolder->getBlob();

		std::string content((char*)blob->getData(), blob->getLength());
		if (content.find("HS") == content.npos)
		{
			return true;
		}

		auto normalizedPath = m_blobHolder->getBlobLoader()->normalizedPath(m_blobHolder);

		unsigned int flag = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		flag |= D3DCOMPILE_DEBUG;
		flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // 

		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), normalizedPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "HS", "hs_5_0",
			flag, 0, &m_hsCompiledBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CompileHullShader Failed:{0}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CompileHullShader {0} Failed:{1}", normalizedPath, "path error");
			}
			SAFE_RELEASE(errorBlob);
			return false;
		}

		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateHullShader(m_hsCompiledBlob->GetBufferPointer(), m_hsCompiledBlob->GetBufferSize(), 0, (void**)&m_hullShader);
		if (FAILED(hr))
		{

			LOG_ERROR("CreateHullShader {0} failed", normalizedPath);
			return false;
		}

		return true;
	}

	bool Renderer::createDomainShader()
	{
		ID3DBlob* errorBlob = nullptr;

		auto blob = m_blobHolder->getBlob();

		std::string content((char*)blob->getData(), blob->getLength());
		if (content.find("DS") == content.npos)
		{
			return true;
		}

		auto normalizedPath = m_blobHolder->getBlobLoader()->normalizedPath(m_blobHolder);

		unsigned int flag = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		flag |= D3DCOMPILE_DEBUG;
		flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // 

		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), normalizedPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "DS", "ds_5_0",
			flag, 0, &m_dsCompiledBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CompileDomainShader Failed:{0}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CompileDomainShader {0} Failed:{1}", normalizedPath, "path error");
			}
			SAFE_RELEASE(errorBlob);
			return false;
		}

		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateDomainShader(m_dsCompiledBlob->GetBufferPointer(), m_dsCompiledBlob->GetBufferSize(), 0, (void**)&m_domainShader);
		if (FAILED(hr))
		{

			LOG_ERROR("CreateDomainShader {0} failed", normalizedPath);
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

						if (SUCCEEDED(hr) && shaderInputBindDesc.Type == D3D_SIT_CBUFFER)
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

				if (shaderInputBindDesc.Type == D3D_SIT_TEXTURE || shaderInputBindDesc.Type == D3D_SIT_STRUCTURED)
				{
					auto iter = m_textures.find(shaderInputBindDesc.Name);
					if (iter == m_textures.end())
					{
						m_textures[shaderInputBindDesc.Name].first = std::make_shared<TextureDesc>();
					}
					m_textures[shaderInputBindDesc.Name].first->startSlot = shaderInputBindDesc.BindPoint;
					m_textures[shaderInputBindDesc.Name].first->textureBindFlag[(TextureBindFlag)flag] = true;
					m_textures[shaderInputBindDesc.Name].second = nullptr;
				}
			}
		}
	}

	void Renderer::collectReflectionSamplerStateInfo(ID3D11ShaderReflection* shaderReflection, short flag)
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
						m_samplerStates[shaderInputBindDesc.Name].first = std::make_shared<SamplerStateDesc>();
					}
					m_samplerStates[shaderInputBindDesc.Name].first->startSlot = shaderInputBindDesc.BindPoint;
					m_samplerStates[shaderInputBindDesc.Name].first->samplerStateBindFlag[(SamplerStateBindFlag)flag] = true;
					m_samplerStates[shaderInputBindDesc.Name].second = nullptr;
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

		if (isLoadingSucceed())
		{
			drawParameters->vertexShader = m_vertexShader;
			drawParameters->pixelShader = m_pixelShader;
			drawParameters->geometryShader = m_geometryShader;
			drawParameters->hullShader = m_hullShader;
			drawParameters->domainShader = m_domainShader;
			drawParameters->inputSignature = m_inputSignatureBlob;
		}
		else
		{
			drawParameters->vertexShader = nullptr;
			drawParameters->pixelShader = nullptr;
			drawParameters->geometryShader = nullptr;
			drawParameters->hullShader = nullptr;
			drawParameters->domainShader = nullptr;
		}

		//drawParameters->constantBuffers = m_constantBuffers;
		//drawParameters->textures = m_textures;
		//drawParameters->samplerStates = m_samplerStates;
	}

	void Renderer::fillConstantBuffers(std::unordered_map<std::string, std::string>& variableLinkConstant, std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& constantBuffers)
	{
		for (const auto& constantBuffer : m_constantBuffers)
		{
			if (!constantBuffer.second)
			{
				continue;
			}

			constantBuffers.insert({ constantBuffer.first, constantBuffer.second->shallowClone() });
		}

		variableLinkConstant = m_variableLinkConstant;
	}

	void Renderer::modifyConstantBuffersByDifference(std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& constantBuffers)
	{
		for (const auto& changedConstantBufferName : m_changedConstantBufferNames)
		{
			auto iter1 = m_variableLinkConstant.find(changedConstantBufferName);
			if (iter1 != m_variableLinkConstant.end())
			{
				auto iter2 = constantBuffers.find(iter1->second);
				if (iter2 != constantBuffers.end())
				{
					iter2->second = m_constantBuffers[iter1->second];
				}
			}
		}

		//m_changedConstantBufferNames.clear();
	}

	void Renderer::fillTextures(std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>& textures)
	{
		textures = m_textures;
	}

	void Renderer::modifyTexturesByDifferenece(std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>& textures)
	{
		for (const auto& changedTextureName : m_changedTextureNames)
		{
			auto iter1 = m_textures.find(changedTextureName);
			if (iter1 != m_textures.end())
			{
				auto iter2 = textures.find(changedTextureName);
				if (iter2 != textures.end())
				{
					iter2->second = iter1->second;
				}
			}
		}
	}

	void Renderer::fillSamplerStates(std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>& samplerStates)
	{
		samplerStates = m_samplerStates;
	}

	void Renderer::modifySamplerStatesByDifferenece(std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>& samplerStates)
	{
		for (const auto& changedSamplerStateNames : m_changedSamplerStateNames)
		{
			auto iter1 = m_samplerStates.find(changedSamplerStateNames);
			if (iter1 != m_samplerStates.end())
			{
				auto iter2 = samplerStates.find(changedSamplerStateNames);
				if (iter2 != samplerStates.end())
				{
					iter2->second = iter1->second;
				}
			}
		}
	}
}