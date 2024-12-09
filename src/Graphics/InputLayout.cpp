#include "InputLayout.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Graphics/GraphicsSystem.h"
#include <vector>
#include <d3d11.h>

namespace Destiny
{
	InputLayout::InputLayout(std::shared_ptr<Blob> inputLayoutDesc) :
		m_inputLayoutDesc(inputLayoutDesc),
		m_inputLayout(nullptr)
	{
		
	}

	InputLayout::~InputLayout()
	{
		SAFE_RELEASE(m_inputLayout);
	}

	ID3D11InputLayout* InputLayout::getInputLayout(std::shared_ptr<Blob> inputSignatureBlob)
	{
		if (m_inputLayout)
		{
			return m_inputLayout;
		}

		if (!inputSignatureBlob)
		{
			return nullptr;
		}

		m_inputLayout = nullptr;

		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateInputLayout
		(
			(D3D11_INPUT_ELEMENT_DESC*)m_inputLayoutDesc->getData(),(UINT) m_inputLayoutDesc->getLength() / sizeof(D3D11_INPUT_ELEMENT_DESC),
			inputSignatureBlob->getData(), inputSignatureBlob->getLength(),
			&m_inputLayout
		);

		if (FAILED(hr))
		{
			LOG_ERROR("Create InputLayout failed");
		}

		return m_inputLayout;
	}

	std::shared_ptr<InputLayout> InputLayout::Create_Position()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		std::shared_ptr<Blob> inputLayoutDesc = std::make_shared<Blob>(sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());
		inputLayoutDesc->copyfrom(inputElements.data(), sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());

		return std::make_shared<InputLayout>(inputLayoutDesc);
	}

	std::shared_ptr<InputLayout> InputLayout::Create_PositionTexcoord()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		std::shared_ptr<Blob> inputLayoutDesc = std::make_shared<Blob>(sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());
		inputLayoutDesc->copyfrom(inputElements.data(), sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());

		return std::make_shared<InputLayout>(inputLayoutDesc);
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
		inputLayoutDesc->copyfrom(inputElements.data(), sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());
		
		return std::make_shared<InputLayout>(inputLayoutDesc);
	}

	std::shared_ptr<InputLayout> InputLayout::Create_PositionColor()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		std::shared_ptr<Blob> inputLayoutDesc = std::make_shared<Blob>(sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());
		inputLayoutDesc->copyfrom(inputElements.data(), sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());

		return std::make_shared<InputLayout>(inputLayoutDesc);
	}
}