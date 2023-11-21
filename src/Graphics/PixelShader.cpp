#include "PixelShader.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	PixelShader::PixelShader() :
		m_pixelShader(nullptr)
	{

	}

	PixelShader::~PixelShader()
	{
		SAFE_RELEASE(m_pixelShader);
	}

	void PixelShader::doLoad()
	{
		if (m_blobHolder)
		{
			if (m_blobHolder->isLoadingPending())
			{
				m_blobHolder->load(0);
			}

			if (!m_blobHolder->isLoadingSucceed())
			{
				LOG_ERROR("PixelShader load failed : {0}", m_blobHolder->getPath());
				loadFailed__();
				return;
			}

			auto blob = m_blobHolder->getBlob();
			if (blob)
			{
				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreatePixelShader(blob->getData(), blob->getLength(), 0, &m_pixelShader);

				if (SUCCEEDED(hr))
				{
					loadSucceeded__();
				}
				else
				{
					LOG_ERROR("PixelShader load failed : {0}", m_blobHolder->getPath());
					loadFailed__();
				}
			}
			else
			{
				LOG_ERROR("PixelShader load failed : {0}", m_blobHolder->getPath());
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