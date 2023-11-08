#include "BlobLoader.h"
#include "BlobHolder.h"

namespace Destiny
{
	BlobLoader::BlobLoader(const std::string& storagePath) :
		m_storagePath(storagePath)
	{

	}

	std::shared_ptr<BlobHolder> BlobLoader::createBlobHolder(const std::string& path)
	{
		return std::make_shared<BlobHolder>(shared_from_this(), path);
	}
}