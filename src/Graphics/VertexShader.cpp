#include "VertexShader.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	VertexShader::VertexShader() :
		m_vertexShader(nullptr)
	{

	}

	VertexShader::~VertexShader()
	{
		SAFE_RELEASE(m_vertexShader);
	}

	void VertexShader::doLoad()
	{
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
				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateVertexShader(blob->getData(), blob->getLength(), 0, &m_vertexShader);

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