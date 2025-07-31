#include "ComputerEffectLoader.h"
#include "ComputerCommand.h"
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
	void ComputerEffectLoader::loadAsset(std::shared_ptr<Asset> asset)
	{
		auto computerCommand = std::dynamic_pointer_cast<ComputerCommand>(asset);
		if (!computerCommand)
		{
			return;
		}

		auto blobHolder = std::dynamic_pointer_cast<BlobHolder>(computerCommand->getCreationParam());
		if (!blobHolder)
		{
			return;
		}

		if (blobHolder->isLoadingPending())
		{
			blobHolder->load(0);
		}

		if (!blobHolder->isLoadingSucceed() || !blobHolder->getBlob())
		{
			asset->loadFailed__();
			return;
		}


		ID3DBlob* errorBlob = nullptr;

		auto blob = blobHolder->getBlob();
		auto normalizedPath = blobHolder->getBlobLoader()->normalizedPath(blobHolder);

		unsigned int flag = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		flag |= D3DCOMPILE_DEBUG;
		flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // 

		HRESULT hr = D3DCompile(blob->getData(), blob->getLength(), normalizedPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_0",
			flag, 0, &computerCommand->m_csCompiledBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("CompileComputerShader Failed:{0}, {1}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()), normalizedPath);
			}
			else
			{
				LOG_ERROR("CompileComputerShader {0} Failed:{1}", normalizedPath, "path error");
			}

			SAFE_RELEASE(errorBlob);
			computerCommand->loadFailed__();
			return;
		}
		LOG_ERROR("CompileComputerShader Succeed:{0}", normalizedPath);
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateComputeShader(computerCommand->m_csCompiledBlob->GetBufferPointer(), computerCommand->m_csCompiledBlob->GetBufferSize(), 0, &computerCommand->m_computeShader);
		if (FAILED(hr))
		{

			LOG_ERROR("CompileComputerShader {0} failed", normalizedPath);
			computerCommand->loadFailed__();
			return;
		}

		collectReflectionInfo(computerCommand->m_csCompiledBlob, computerCommand);

		computerCommand->loadSucceeded__();
	}

	void ComputerEffectLoader::collectReflectionInfo(ID3D10Blob* compiledBlob, std::shared_ptr<ComputerCommand> computerCommand)
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
			collectReflectionConstantInfo(shaderReflection, computerCommand);
			collectReflectionTextureInfo(shaderReflection, computerCommand);
			//collectReflectionSamplerStateInfo(shaderReflection, flag);
		}
	}

	void ComputerEffectLoader::collectReflectionConstantInfo(ID3D11ShaderReflection* shaderReflection, std::shared_ptr<ComputerCommand> computerCommand)
	{
		if (!shaderReflection || !computerCommand)
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
							auto iter = computerCommand->m_constantBuffers.find(shaderBufferDesc.Name);
							if (iter == computerCommand->m_constantBuffers.end())
							{
								computerCommand->m_constantBuffers[shaderBufferDesc.Name] = std::make_shared<ConstantBuffer>(shaderInputBindDesc.BindPoint, shaderBufferDesc.Size);
								for (unsigned int j = 0; j < shaderBufferDesc.Variables; j++)
								{
									ID3D11ShaderReflectionVariable* shaderReflectionVariable = reflectionConstantBuffer->GetVariableByIndex(j);
									if (shaderReflectionVariable)
									{
										D3D11_SHADER_VARIABLE_DESC shaderVariableDesc;
										hr = shaderReflectionVariable->GetDesc(&shaderVariableDesc);
										if (SUCCEEDED(hr))
										{
											computerCommand->m_constantBuffers[shaderBufferDesc.Name]->addVariable(shaderVariableDesc.Name, { shaderVariableDesc.StartOffset, shaderVariableDesc.Size });
											computerCommand->m_variableLinkConstant[shaderVariableDesc.Name] = shaderBufferDesc.Name;
										}
									}
								}
							}
							computerCommand->m_constantBuffers[shaderBufferDesc.Name]->setConstantBufferBindFlag(ConstantBufferBindFlag::BindCS, true);
						}
					}
				}
			}
		}
	}

	void ComputerEffectLoader::collectReflectionTextureInfo(ID3D11ShaderReflection* shaderReflection, std::shared_ptr<ComputerCommand> computerCommand)
	{
		if (!shaderReflection || !computerCommand)
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
					auto iter = computerCommand->m_srvs.find(shaderInputBindDesc.Name);
					if (iter == computerCommand->m_srvs.end())
					{
						computerCommand->m_srvs[shaderInputBindDesc.Name].first = std::make_shared<TextureDesc>();
					}
					computerCommand->m_srvs[shaderInputBindDesc.Name].first->startSlot = shaderInputBindDesc.BindPoint;
					computerCommand->m_srvs[shaderInputBindDesc.Name].first->textureBindFlag[TextureBindFlag::BindCS] = true;
					computerCommand->m_srvs[shaderInputBindDesc.Name].second = nullptr;
				}
			}
		}
	}
}
