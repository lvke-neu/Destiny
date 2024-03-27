#include "InputLayout.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Graphics/GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	InputLayout::InputLayout(std::shared_ptr<Blob> inputLayoutDesc, std::shared_ptr<Blob> inputSignatureBlob) :
		m_inputLayoutDesc(inputLayoutDesc),
		m_inputSignatureBlob(inputSignatureBlob)
	{

	}

	InputLayout::~InputLayout()
	{
		SAFE_RELEASE(m_inputLayout);
	}

	void InputLayout::doLoad()
	{
		if (!m_inputLayoutDesc || !m_inputSignatureBlob)
		{
			loadFailed__();
			LOG_ERROR("CreateInputLayout failed");
			m_inputLayoutDesc.reset();
			m_inputSignatureBlob.reset();
			return;
		}

		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateInputLayout(
			(D3D11_INPUT_ELEMENT_DESC*)m_inputLayoutDesc->getData(), (unsigned int)m_inputLayoutDesc->getLength() / sizeof(D3D11_INPUT_ELEMENT_DESC),
			m_inputSignatureBlob->getData(), (unsigned int)m_inputSignatureBlob->getLength(), &m_inputLayout);

		if (SUCCEEDED(hr))
		{
			loadSucceeded__();
			m_inputLayoutDesc.reset();
			m_inputSignatureBlob.reset();
		}
		else
		{
			LOG_ERROR("CreateInputLayout failed");
			loadFailed__();
			m_inputLayoutDesc.reset();
			m_inputSignatureBlob.reset();
		}
	}

	std::shared_ptr<InputLayout> InputLayout::Create_PositionNormalTexcoord(std::shared_ptr<Blob> inputSignatureBlob)
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[3] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		std::shared_ptr<Blob> inputLayoutDesc = std::make_shared<Blob>(sizeof(D3D11_INPUT_ELEMENT_DESC) * 3);
		memcpy_s(inputLayoutDesc->getData(), inputLayoutDesc->getLength(), inputElements, inputLayoutDesc->getLength());

		return std::make_shared<InputLayout>(inputLayoutDesc, inputSignatureBlob);
	}
}