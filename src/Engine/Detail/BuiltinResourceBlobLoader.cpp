#include "BuiltinResourceBlobLoader.h"
#include "../BlobHolder.h"
#include "../Blob.h"
#include "../Utility.h"
#include <thread>
#include <fstream>
#include <Windows.h>


namespace Destiny
{

	BuiltinResourceBlobLoader::BuiltinResourceBlobLoader() :
		BlobLoader("builtin://")
	{

	}

	void BuiltinResourceBlobLoader::doLoad(std::shared_ptr<BlobHolder> blobHolder)
	{
		m_mtx.lock();

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
			LOG_ERROR("Thread {0}, BuiltinResourceBlobLoader failed : {1}", std::to_string((*(uint32_t*)&std::this_thread::get_id())), blobHolder->getPath());
			m_mtx.unlock();
			return;
		}

		ifs.seekg(0, std::ios::end);
		auto length = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		std::shared_ptr<Blob> blob(new Blob(length));

		ifs.read((char*)blob->getData(), blob->getLength());

		ifs.close();

		blobHolder->loadSucceeded__(blob);

		m_mtx.unlock();
	}
}