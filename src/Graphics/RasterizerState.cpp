#include "RasterizerState.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	D3D11_RASTERIZER_DESC RasterizerState::Default_Rasterizer_Desc
	{
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK,
		false,
		0,
		0,
		0,
		true,
		false,
		false,
		false
	};

	RasterizerState::RasterizerState() :
		m_rasterizerState(nullptr)
	{

	}

	RasterizerState::~RasterizerState()
	{
		SAFE_RELEASE(m_rasterizerState);
	}

	void RasterizerState::doLoad()
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
				D3D11_RASTERIZER_DESC desc;
				memcpy_s(&desc, sizeof(D3D11_RASTERIZER_DESC), blob->getData(), sizeof(D3D11_RASTERIZER_DESC));

				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateRasterizerState(&desc, &m_rasterizerState);

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