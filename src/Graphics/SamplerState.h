#pragma once
#include "Engine/Asset.h"
#include <unordered_map>

struct ID3D11SamplerState;
struct D3D11_SAMPLER_DESC;
namespace Destiny
{
	enum class SamplerStateBindFlag
	{
		BindVS,
		BindPS,
		BindGS
	};

	struct SamplerStateDesc
	{
		SamplerStateDesc() :
			startSlot(-1)
		{
			samplerStateBindFlag =
			{
				{SamplerStateBindFlag::BindVS, false },
				{SamplerStateBindFlag::BindPS, false },
				{SamplerStateBindFlag::BindGS, false }
			};
		}
		std::unordered_map<SamplerStateBindFlag, bool> samplerStateBindFlag;
		unsigned int startSlot;
	};

	class SamplerState : public Asset
	{
	public:
		SamplerState();
		virtual ~SamplerState();
	public:
		virtual void doLoad() override;
	public:
		void bind(std::shared_ptr<SamplerStateDesc> desc);
		std::shared_ptr<D3D11_SAMPLER_DESC> getSamplerDesc();
	private:
		ID3D11SamplerState* m_samplerState;
		std::shared_ptr<D3D11_SAMPLER_DESC> m_samplerDesc;
	};

	inline std::shared_ptr<D3D11_SAMPLER_DESC> SamplerState::getSamplerDesc()
	{
		return m_samplerDesc;
	}
}