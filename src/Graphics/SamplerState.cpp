#include "SamplerState.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	D3D11_SAMPLER_DESC SamplerState::Default_SamplerState_Desc
	{
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		0.0f,
		0,
		D3D11_COMPARISON_NEVER,
		{0.0f, 0.0f, 0.0f, 0.0f},
		-FLT_MAX,
		FLT_MAX
	};

	SamplerState::SamplerState() :
		m_samplerState(nullptr)
	{

	}

	SamplerState::~SamplerState()
	{
		SAFE_RELEASE(m_samplerState);
	}

	void SamplerState::doLoad()
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
				D3D11_SAMPLER_DESC desc;
				memcpy_s(&desc, sizeof(D3D11_SAMPLER_DESC), blob->getData(), sizeof(D3D11_SAMPLER_DESC));

				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateSamplerState(&desc, &m_samplerState);

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