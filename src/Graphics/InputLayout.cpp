#include "InputLayout.h"
#include "Engine/Blob.h"
#include <vector>
#include <d3d11.h>

namespace Destiny
{
	InputLayout::InputLayout(std::shared_ptr<Blob> inputLayoutDesc) :
		m_inputLayoutDesc(inputLayoutDesc)
	{

	}

	std::shared_ptr<InputLayout> InputLayout::Create_PositionNormalTexcoord()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		std::shared_ptr<Blob> inputLayoutDesc = std::make_shared<Blob>(sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());
		memcpy_s(inputLayoutDesc->getData(), inputLayoutDesc->getLength(), inputElements.data(), inputLayoutDesc->getLength());

		return std::make_shared<InputLayout>(inputLayoutDesc);
	}
}