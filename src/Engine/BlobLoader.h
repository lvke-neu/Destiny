#pragma once
#include <string>
#include <memory>
#include <mutex>

namespace Destiny
{
	class BlobHolder;
	class BlobLoader : public std::enable_shared_from_this<BlobLoader>
	{
		friend class BlobLoaderManager;
	public:
		BlobLoader(const std::string& storagePath);
		virtual ~BlobLoader() = default;
	public:
		virtual std::shared_ptr<BlobHolder> createBlobHolder(const std::string& path);
		virtual void doLoad(std::shared_ptr<BlobHolder> blobHolder) = 0;
		virtual std::string normalizedPath(std::shared_ptr<BlobHolder> blobHolder) = 0;
	protected:
		std::string		m_storagePath;
		std::mutex		m_mtx;
	};
}