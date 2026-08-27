#include "BlobLoaderManager.h"
#include "Utility.h"
#include "Detail/BuiltinResourceBlobLoader.h"
#include "Detail/HttpBlobLoader.h"

namespace Destiny
{
	BlobLoaderManager::BlobLoaderManager()
	{
		registerBlobLoader(std::make_shared<BuiltinResourceBlobLoader>());
		registerBlobLoader(std::make_shared<HttpBlobLoader>("http://"));
		registerBlobLoader(std::make_shared<HttpBlobLoader>("https://"));
	}

	void BlobLoaderManager::registerBlobLoader(std::shared_ptr<BlobLoader> blobLoader)
	{
		if (!blobLoader)
		{
			return;
		}

		auto it = m_blobLoaders.begin();
		auto end = m_blobLoaders.end();
		for (; it != end; ++it)
		{
			if ((*it)->m_storagePath == blobLoader->m_storagePath)
			{
				return;
			}
		}
		m_blobLoaders.push_back(blobLoader);
	}

	std::shared_ptr<BlobLoader> BlobLoaderManager::getBlobLoader(const char* path)
	{
		auto it = m_blobLoaders.begin();
		auto end = m_blobLoaders.end();
		for (; it != end; ++it)
		{
			std::shared_ptr<BlobLoader> blobLoader = *it;
			if (0 == strncmp(blobLoader->m_storagePath.c_str(), path, blobLoader->m_storagePath.size()))
			{
				return blobLoader;
			}
		}

		LOG_ERROR("GetBlobLoader failed:{0}", path);

		return nullptr;
	}
}