#include "VertexBufferLoader.h"
#include "VertexBuffer.h"
#include "VertexBufferFile.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"

namespace Destiny
{
	void VertexBufferLoader::loadAsset(std::shared_ptr<Asset> asset)
	{
		m_mtx.lock();

		std::shared_ptr<BlobHolder> blobHolder = asset->getBlobHolder();
		if (!blobHolder)
		{
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}

		if (blobHolder->isLoadingPending())
		{
			blobHolder->load(0);
		}
		
		if (blobHolder->isLoadingSucceed())
		{
			std::shared_ptr<Blob> blob = blobHolder->getBlob();
			if (blob)
			{
				if (VertexBufferFile::ParseXml(blob->getData(), (unsigned int)blob->getLength()))
				{
					asset->loadSucceeded__();
				}
				else
				{
					asset->loadFailed__();
				}
			}
			else
			{
				asset->loadFailed__();
			}
		}
		else
		{
			asset->loadFailed__();
		}
		m_mtx.unlock();
	}

	std::shared_ptr<Asset> VertexBufferLoader::createAsset(std::shared_ptr<BlobHolder> blobHolder)
	{
		std::shared_ptr<Asset> asset(new VertexBuffer);
		asset->initialize(shared_from_this(), blobHolder);
		return asset;
	}
}