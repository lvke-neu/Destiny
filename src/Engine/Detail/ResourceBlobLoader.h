#pragma once
#include "../BlobLoader.h"

namespace Destiny
{
	class ResourceBlobLoader : BlobLoader
	{
	public:
		ResourceBlobLoader();
	public:
		virtual void doLoad(std::shared_ptr<BlobHolder> blobHolder) override;
	};
}