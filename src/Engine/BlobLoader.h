#pragma once
#include <string>
#include <memory>

namespace Destiny
{
	class BlobHolder;
	class BlobLoader
	{
		friend class BlobLoaderManager;
	public:
		BlobLoader(const std::string& storagePath);
		virtual ~BlobLoader() = default;
	public:
		virtual BlobHolder* createBlobHolder(const std::string& path);
		virtual void doLoad(std::shared_ptr<BlobHolder> blobHolder) = 0;
	protected:
		std::string m_storagePath;
	};
}