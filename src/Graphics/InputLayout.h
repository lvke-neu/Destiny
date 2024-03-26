#pragma once
#include "Engine/Asset.h"

struct ID3D11InputLayout;
namespace Destiny
{
	class InputLayout : public Asset
	{
	public:
		InputLayout();
		virtual ~InputLayout();
	public:
		virtual void doLoad() override;
	private:
		ID3D11InputLayout* m_inputLayout;
	};

}