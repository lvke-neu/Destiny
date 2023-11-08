#include "BlobLoader.h"

namespace Destiny
{
	BlobLoader::BlobLoader(const std::string& storagePath) :
		m_storagePath(storagePath)
	{

	}

	BlobHolder* BlobLoader::createBlobHolder(const std::string& path)
	{
		//TODO:
		return nullptr;
	}
}