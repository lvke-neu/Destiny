#pragma once
#include "BlobLoader.h"

namespace Destiny
{
	class AssetBlobLoader : BlobLoader
	{
	public:
		AssetBlobLoader();
	public:
		virtual void doLoad(std::shared_ptr<BlobHolder> blobHolder) override;
	};
}