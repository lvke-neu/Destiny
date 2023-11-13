#include "InputLayout.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	InputLayout::InputLayout(std::shared_ptr<Blob> inputElements) :
		m_inputElements(inputElements),
		m_inputLayout(nullptr)
	{

	}

	InputLayout::~InputLayout()
	{
		SAFE_RELEASE(m_inputLayout);
	}

	void InputLayout::doLoad()
	{
		if (!m_inputElements)
		{
			loadFailed__();
			return;
		}

		if (m_blobHolder)
		{
			if (m_blobHolder->isLoadingPending())
			{
				m_blobHolder->load(0);
			}

			if (!m_blobHolder->isLoadingSucceed())
			{
				loadFailed__();
				return;
			}

			auto blob = m_blobHolder->getBlob();
			if (blob)
			{
				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateInputLayout(
					(D3D11_INPUT_ELEMENT_DESC*)m_inputElements->getData(), (unsigned int)m_inputElements->getLength() / sizeof(D3D11_INPUT_ELEMENT_DESC),
					blob->getData(), (unsigned int)blob->getLength(), &m_inputLayout);

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
				return;
			}

		}
		else
		{
			loadFailed__();
		}
	}
}