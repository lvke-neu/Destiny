#include "ComputerCommand.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Texture.h"
#include "ComputerEffectLoader.h"
#include <d3d11.h>
#include <d3dcompiler.h>

namespace Destiny
{
	std::shared_ptr<ComputerEffectLoader> ComputerCommand::s_computerEffectLoader = std::make_shared<ComputerEffectLoader>();
	ComputerCommand::ComputerCommand() :
		m_computeShader(nullptr),
		m_csCompiledBlob(nullptr),
		m_threadGroupCountX(0),
		m_threadGroupCountY(0),
		m_threadGroupCountZ(0),
		m_debugName(L""),
		m_bIndirectMode(false),
		m_indirectTexture(nullptr)
	{

	}

	ComputerCommand::~ComputerCommand()
	{
		SAFE_RELEASE(m_computeShader);
		SAFE_RELEASE(m_csCompiledBlob);
	}

	void ComputerCommand::execute(std::shared_ptr<GraphicsContext> deviceContext)
	{
#if 0
		////clear append type structed uav
		//UINT clearValues[4] = { 0, 0, 0, 0xffffffff };
		//for (int i = 0; i < m_uavs.size(); i++)
		//{
		//	if (m_uavs[i])
		//	{
		//		auto creationParam = std::static_pointer_cast<TextureCreationParam>(m_uavs[i]->getCreationParam());
		//		if (creationParam && creationParam->createStructuredType == TextureCreationParam::CreateStructuredType::Counter)
		//		{
		//			deviceContext->ClearUnorderedAccessViewUint(*m_uavs[i]->getUnorderedAccessView(), clearValues);
		//		}	
		//	}
		//}
		
		if (!m_debugName.empty())
		{
			Engine::GetInstance()->getGraphicsSystem()->beginEvent(m_debugName.c_str());
		}
		
		UINT clearCounts[1] = { 0 };
		deviceContext->CSSetShader(m_computeShader, nullptr, 0);
		if (m_uavs.size() == m_clearUint.size())
		{
			for (int i = 0; i < m_uavs.size(); i++)
			{
				if (m_uavs[i])
				{
					if (m_clearUint[i] == 0)
					{
						deviceContext->CSSetUnorderedAccessViews(i, 1, m_uavs[i]->getUnorderedAccessView(), clearCounts);
					}
					else
					{
						deviceContext->CSSetUnorderedAccessViews(i, 1, m_uavs[i]->getUnorderedAccessView(), nullptr);
					}
				}
			}
		}

		for (const auto& srv : m_srvs)
		{
			if (srv.second.second)
			{
				srv.second.second->bind(srv.second.first);
			}
		}

		for (const auto& constantBuffer : m_constantBuffers)
		{
			if (constantBuffer.second)
			{
				constantBuffer.second->bind();
			}
		}

		if (!m_bIndirectMode)
		{
			deviceContext->Dispatch(m_threadGroupCountX, m_threadGroupCountY, m_threadGroupCountZ);
		}
		else
		{
			if (m_indirectTexture)
			{
				for (const auto& indirectOffset : m_indirectOffsets)
				{
					deviceContext->DispatchIndirect((ID3D11Buffer*)m_indirectTexture->m_resource, indirectOffset);
				}
			}
		}
		
		for (const auto& srv : m_srvs)
		{
			if (srv.second.second)
			{
				srv.second.second->unBind(srv.second.first);
			}
		}

		for (const auto& constantBuffer : m_constantBuffers)
		{
			if (constantBuffer.second)
			{
				constantBuffer.second->unBind();
			}
		}

		ID3D11UnorderedAccessView* uav = nullptr;
		deviceContext->CSSetShader(nullptr, nullptr, 0);
		for (int i = 0; i < m_uavs.size(); i++)
		{
			if (m_uavs[i])
			{
				deviceContext->CSSetUnorderedAccessViews(i, 1, &uav, nullptr);
			}
		}

		if (!m_debugName.empty())
		{
			Engine::GetInstance()->getGraphicsSystem()->endEvent();
		}
#endif
	}

	void ComputerCommand::setDebugName(const std::wstring& debugName)
	{
		m_debugName = debugName;
	}

	void ComputerCommand::setComputerEffectPath(const char* path)
	{
		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			auto blobHolder = blobLoader->createBlobHolder(path);
			initialize(s_computerEffectLoader, blobHolder);
		}

		load(0);
	}

	void ComputerCommand::setThreadGroupCount(unsigned int threadGroupCountX, unsigned int threadGroupCountY, unsigned int threadGroupCountZ)
	{
		m_threadGroupCountX = threadGroupCountX;
		m_threadGroupCountY = threadGroupCountY;
		m_threadGroupCountZ = threadGroupCountZ;
	}

	std::shared_ptr<Texture> ComputerCommand::getUnorderedAccessViews(int index)
	{
		if (index < 0 || index >= m_uavs.size())
		{
			return nullptr;
		}
		return m_uavs[index];
	}

	void ComputerCommand::setUnorderedAccessViews(const std::vector<std::shared_ptr<Texture>>& uavs, const std::vector<unsigned int>& clearUint)
	{
		m_uavs = uavs;
		m_clearUint = clearUint;
	}

	void ComputerCommand::setShaderResourceView(const char* name, std::shared_ptr<Texture> texture)
	{
		auto iter = m_srvs.find(name);
		if (iter == m_srvs.end())
		{
			return;
		}

		iter->second.second = texture;
	}

	void ComputerCommand::setIndirectMode(bool bIndirectMode, std::shared_ptr<Texture> indirectTexture, std::vector<unsigned int> indirectOffsets)
	{
		m_bIndirectMode = bIndirectMode;
		m_indirectTexture = indirectTexture;
		m_indirectOffsets = indirectOffsets;
	}

	void ComputerCommand::doLoad()
	{
		auto blobHolder = std::dynamic_pointer_cast<BlobHolder>(m_creationParam);
		if (!blobHolder)
		{
			loadFailed__();
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
			flag, 0, &m_csCompiledBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				LOG_ERROR("ComputerCommand:compile error:{0}", (char*)errorBlob->GetBufferPointer());
			}
			loadFailed__();
			SAFE_RELEASE(errorBlob);
			return;
		}

		hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateComputeShader(m_csCompiledBlob->GetBufferPointer(), m_csCompiledBlob->GetBufferSize(), nullptr, (void**)&m_computeShader);
		if (FAILED(hr))
		{
			LOG_ERROR("ComputerCommand:create compute shader failed");
			loadFailed__();
			SAFE_RELEASE(errorBlob);
			return;
		}

		loadSucceeded__();
		SAFE_RELEASE(errorBlob);
	}
}
