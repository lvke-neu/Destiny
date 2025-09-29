#pragma once
#include "../BlobLoader.h"

namespace Destiny
{
	class HttpBlobLoader : public BlobLoader
	{
	public:
		HttpBlobLoader(const std::string& storagePath);
		virtual ~HttpBlobLoader();
	public:
		virtual void			doLoad(std::shared_ptr<BlobHolder> blobHolder) override;
		virtual std::string		normalizedPath(std::shared_ptr<BlobHolder> blobHolder) override;
	};
}