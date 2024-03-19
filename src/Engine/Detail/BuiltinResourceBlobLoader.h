#pragma once
#include "../BlobLoader.h"

namespace Destiny
{
	class BuiltinResourceBlobLoader : BlobLoader
	{
	public:
		BuiltinResourceBlobLoader();
	public:
		virtual void doLoad(std::shared_ptr<BlobHolder> blobHolder) override;
	};
}