#pragma once
#include "../BlobLoader.h"

namespace Destiny
{
	class BuiltinResourceBlobLoader : public BlobLoader
	{
	public:
		BuiltinResourceBlobLoader();
	public:
		virtual void doLoad(std::shared_ptr<BlobHolder> blobHolder) override;
		virtual std::string normalizedPath(std::shared_ptr<BlobHolder> blobHolder) override;
	};
}