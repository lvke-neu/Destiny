#include "AssetBlobLoader.h"
#include "BlobHolder.h"
#include "Blob.h"
#include <fstream>
#include <Windows.h>


namespace Destiny
{

	AssetBlobLoader::AssetBlobLoader() : 
		BlobLoader("assets://")
	{

	}

	void AssetBlobLoader::doLoad(std::shared_ptr<BlobHolder> blobHolder)
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, sizeof(buffer));

		std::string exePath = buffer;
		auto pos = exePath.find("Destiny");
		if (pos != exePath.npos)
		{
			exePath = exePath.substr(0, pos + 7);
		}
		
		std::string path = exePath + "\\assets\\" + blobHolder->getPath();

		std::ifstream ifs;
		ifs.open(path, std::ios::in | std::ios::binary);
		if (!ifs.is_open())
		{
			blobHolder->loadFailed__();
			return;
		}

		ifs.seekg(0, std::ios::end);
		auto length = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		std::shared_ptr<Blob> blob(new Blob(length));

		ifs.read((char*)blob->getData(), blob->getLength());

		ifs.close();

		blobHolder->loadSucceeded__(blob);
	}
}