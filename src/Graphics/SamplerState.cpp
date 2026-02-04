#include "SamplerState.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	SamplerState::SamplerState() :
		m_samplerState(nullptr),
		m_samplerDesc(std::make_shared<CD3D11_SAMPLER_DESC>(CD3D11_DEFAULT()))
	{

	}

	SamplerState::~SamplerState()
	{
		// SAFE_RELEASE(m_samplerState);
        if (m_samplerState)
        {
             Engine::GetInstance()->getGraphicsSystem()->getDevice()->DestroySamplerState(m_samplerState);
             m_samplerState = nullptr;
        }
	}

	void SamplerState::doLoad()
	{
		if (m_samplerDesc)
		{
			
			HRESULT hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateSamplerState(m_samplerDesc.get(), (void**)&m_samplerState);
			
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
		}
	}

	void SamplerState::bind(std::shared_ptr<SamplerStateDesc> desc)
	{
		if (!desc)
		{
			return;
		}

		for (const auto& samplerStateBindFlag : desc->samplerStateBindFlag)
		{
			if (samplerStateBindFlag.second)
			{
				switch (samplerStateBindFlag.first)
				{
				case SamplerStateBindFlag::BindVS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetSamplers(desc->startSlot, 1, (void* const*)&m_samplerState);
					break;
				case SamplerStateBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetSamplers(desc->startSlot, 1, (void* const*)&m_samplerState);
					break;
				case SamplerStateBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetSamplers(desc->startSlot, 1, (void* const*)&m_samplerState);
					break;
				case SamplerStateBindFlag::BindHS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->HSSetSamplers(desc->startSlot, 1, (void* const*)&m_samplerState);
					break;
				case SamplerStateBindFlag::BindDS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->DSSetSamplers(desc->startSlot, 1, (void* const*)&m_samplerState);
					break;
				case SamplerStateBindFlag::BindCS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->CSSetSamplers(desc->startSlot, 1, (void* const*)&m_samplerState);
					break;
				}
			}
		}
	}

	void SamplerState::unBind(std::shared_ptr<SamplerStateDesc> desc)
	{
		if (!desc)
		{
			return;
		}

		void* sampler = nullptr;
		for (const auto& samplerStateBindFlag : desc->samplerStateBindFlag)
		{
			if (samplerStateBindFlag.second)
			{
				switch (samplerStateBindFlag.first)
				{
				case SamplerStateBindFlag::BindVS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetSamplers(desc->startSlot, 1, (void* const*)&sampler);
					break;
				case SamplerStateBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetSamplers(desc->startSlot, 1, (void* const*)&sampler);
					break;
				case SamplerStateBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetSamplers(desc->startSlot, 1, (void* const*)&sampler);
					break;
				case SamplerStateBindFlag::BindHS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->HSSetSamplers(desc->startSlot, 1, (void* const*)&sampler);
					break;
				case SamplerStateBindFlag::BindDS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->DSSetSamplers(desc->startSlot, 1, (void* const*)&sampler);
					break;
				}
			}
		}
	}
}