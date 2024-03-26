#pragma once
#include "Engine/Asset.h"

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
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