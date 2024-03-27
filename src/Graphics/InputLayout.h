#pragma once
#include "Engine/Asset.h"

struct ID3D11InputLayout;
namespace Destiny
{
	class Blob;
	class InputLayout : public Asset
	{
	public:
		//D3D11_INPUT_ELEMENT_DESC* memcpy in this class, manage this pointer in your scope
		InputLayout(std::shared_ptr<Blob> inputLayoutDesc, std::shared_ptr<Blob> inputSignatureBlob);
		virtual ~InputLayout();
	public:
		virtual void doLoad() override;
	public:
		static std::shared_ptr<InputLayout> Create_PositionNormalTexcoord(std::shared_ptr<Blob> inputSignatureBlob);
	private:
		ID3D11InputLayout* m_inputLayout;
		std::shared_ptr<Blob> m_inputLayoutDesc;
		std::shared_ptr<Blob> m_inputSignatureBlob;
	};
}