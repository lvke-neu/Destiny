#pragma once
#include <vector>
#include <memory>
#include <string>

namespace Destiny
{
	class BlobLoader;
	class BlobLoaderManager
	{
	public:
		void registerBlobLoader(std::shared_ptr<BlobLoader> blobLoader);
		std::shared_ptr<BlobLoader> getBlobLoader(const std::string& path);
	private:
		std::vector<std::shared_ptr<BlobLoader>> m_blobLoaders;
	};
}