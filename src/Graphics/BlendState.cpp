#include "BlendState.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	D3D11_BLEND_DESC BlendState::Default_BlendState_Desc
	{
		false,
		false,
		{
			{
				false,
				D3D11_BLEND_ONE,
				D3D11_BLEND_ZERO,
				D3D11_BLEND_OP_ADD,
				D3D11_BLEND_ONE,
				D3D11_BLEND_ZERO,
				D3D11_BLEND_OP_ADD,
				15
			}
		}
	};

	BlendState::BlendState() :
		m_blendState(nullptr)
	{

	}

	BlendState::~BlendState()
	{
		SAFE_RELEASE(m_blendState);
	}

	void BlendState::doLoad()
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
				D3D11_BLEND_DESC desc;
				memcpy_s(&desc, sizeof(D3D11_BLEND_DESC), blob->getData(), sizeof(D3D11_BLEND_DESC));

				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBlendState(&desc, &m_blendState);

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