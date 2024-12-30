#pragma once
#include <memory>

struct ID3D11InputLayout;
namespace Destiny
{
	class Blob;
	class InputLayout
	{
	public:
		InputLayout(std::shared_ptr<Blob> inputLayoutDesc);
		~InputLayout();
	public:
		ID3D11InputLayout*						getInputLayout(std::shared_ptr<Blob> inputSignatureBlob);
	public:
		static std::shared_ptr<InputLayout>		Create_Position3();
		static std::shared_ptr<InputLayout>		Create_Position2();
		static std::shared_ptr<InputLayout>		Create_PositionTexcoord();
		static std::shared_ptr<InputLayout>		Create_PositionNormalTexcoord();
		static std::shared_ptr<InputLayout>		Create_PositionColor();
	private:
		std::shared_ptr<Blob>	m_inputLayoutDesc;
		ID3D11InputLayout*		m_inputLayout;
	};
}