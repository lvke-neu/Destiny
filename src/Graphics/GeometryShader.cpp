#include "GeometryShader.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	GeometryShader::GeometryShader() :
		m_geometryShader(nullptr)
	{

	}

	GeometryShader::~GeometryShader()
	{
		SAFE_RELEASE(m_geometryShader);
	}

	void GeometryShader::doLoad()
	{
		if (m_blobHolder)
		{
			if (m_blobHolder->isLoadingPending())
			{
				m_blobHolder->load(0);
			}

			if (!m_blobHolder->isLoadingSucceed())
			{
				LOG_ERROR("GeometryShader load failed : {0}", m_blobHolder->getPath());
				loadFailed__();
				return;
			}

			auto blob = m_blobHolder->getBlob();
			if (blob)
			{
				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateGeometryShader(blob->getData(), blob->getLength(), 0, &m_geometryShader);

				if (SUCCEEDED(hr))
				{
					loadSucceeded__();
				}
				else
				{
					LOG_ERROR("GeometryShader load failed : {0}", m_blobHolder->getPath());
					loadFailed__();
				}
			}
			else
			{
				LOG_ERROR("GeometryShader load failed : {0}", m_blobHolder->getPath());
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