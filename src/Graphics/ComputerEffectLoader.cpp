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
				LOG_ERROR("CompileComputerShader Failed:{0}", reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			else
			{
				LOG_ERROR("CompileComputerShader {0} Failed:{1}", normalizedPath, "path error");
			}
			SAFE_RELEASE(errorBlob);
			computerCommand->loadFailed__();
			return;
		}

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
			//collectReflectionConstantInfo(shaderReflection, flag);
			collectReflectionTextureInfo(shaderReflection, computerCommand);
			//collectReflectionSamplerStateInfo(shaderReflection, flag);
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
