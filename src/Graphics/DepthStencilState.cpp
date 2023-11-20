#include "DepthStencilState.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	D3D11_DEPTH_STENCIL_DESC DepthStencilState::Default_DepthStencil_Desc
	{
		true,
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS,
		false,
		255,
		255,
		{
			D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP,
			D3D11_COMPARISON_ALWAYS
		},
		{
			D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP,
			D3D11_COMPARISON_ALWAYS
		}

	};

	DepthStencilState::DepthStencilState() :
		m_depthStencilState(nullptr)
	{

	}

	DepthStencilState::~DepthStencilState()
	{
		SAFE_RELEASE(m_depthStencilState);
	}

	void DepthStencilState::doLoad()
	{
		if (m_blobHolder)
		{
			if (m_blobHolder->isLoadingPending())
			{
				m_blobHolder->load(0);
			}

			if (!m_blobHolder->isLoadingSucceed())
			{
				loadFailed__();
				return;
			}

			auto blob = m_blobHolder->getBlob();
			if (blob)
			{
				D3D11_DEPTH_STENCIL_DESC desc;
				memcpy_s(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC), blob->getData(), sizeof(D3D11_DEPTH_STENCIL_DESC));

				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateDepthStencilState(&desc, &m_depthStencilState);

				if (SUCCEEDED(hr))
				{
					loadSucceeded__();
				}
				else
				{
					loadFailed__();
				}
			}
			else
			{
				loadFailed__();
				return;
			}
		}
		else
		{
			loadFailed__();
		}
	}
}