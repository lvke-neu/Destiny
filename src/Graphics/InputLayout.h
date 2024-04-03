#pragma once
#include <memory>

namespace Destiny
{
	class Blob;
	class InputLayout
	{
	public:
		InputLayout(std::shared_ptr<Blob> inputLayoutDesc);
	public:
		static std::shared_ptr<InputLayout> Create_PositionNormalTexcoord();
	private:
		std::shared_ptr<Blob>	m_inputLayoutDesc;
	};
}