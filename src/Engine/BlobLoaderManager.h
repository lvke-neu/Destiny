#pragma once
#include <vector>
#include <memory>

namespace Destiny
{
	class BlobLoader;
	class BlobLoaderManager
	{
	public:
		BlobLoaderManager();
	public:
		void										registerBlobLoader(std::shared_ptr<BlobLoader> blobLoader);
		std::shared_ptr<BlobLoader>					getBlobLoader(const char* path);
	private:
		std::vector<std::shared_ptr<BlobLoader>>	m_blobLoaders;
	};
}