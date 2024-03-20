#include "BlobLoaderManager.h"
#include "BlobLoader.h"
#include "Utility.h"
#include "Detail/BuiltinResourceBlobLoader.h"

namespace Destiny
{
	BlobLoaderManager::BlobLoaderManager()
	{
		registerBlobLoader(std::shared_ptr<BlobLoader>((BlobLoader*)new BuiltinResourceBlobLoader));
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

	std::shared_ptr<BlobLoader> BlobLoaderManager::getBlobLoader(const std::string& path)
	{
		auto it = m_blobLoaders.begin();
		auto end = m_blobLoaders.end();
		for (; it != end; ++it)
		{
			std::shared_ptr<BlobLoader> blobLoader = *it;
			if (0 == strncmp(blobLoader->m_storagePath.c_str(), path.c_str(), blobLoader->m_storagePath.size()))
			{
				return blobLoader;
			}
		}

		LOG_ERROR("GetBlobLoader failed:{0}", path);

		return nullptr;
	}
}