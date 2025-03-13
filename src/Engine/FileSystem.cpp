#include "FileSystem.h"
#include <fstream>
#include "Blob.h"

namespace Destiny
{
	std::shared_ptr<Blob> FileSystem::ReadBlob(const char* path)
	{
		std::ifstream ifs;
		ifs.open(path, std::ios::in | std::ios::binary);
		if (!ifs.is_open())
		{
			return nullptr;
		}

		ifs.seekg(0, std::ios::end);
		auto length = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		std::shared_ptr<Blob> blob(new Blob(length));

		ifs.read((char*)blob->getData(), blob->getLength());

		ifs.close();

		return blob;
	}

	void FileSystem::WriteBlob(const char* path, std::shared_ptr<Blob> blob)
	{
		if (!blob)
		{
			return;
		}

		std::ofstream ofs(path, std::ios::binary);

		if (ofs.is_open())
		{
			ofs.write((char*)blob->getData(), blob->getLength());
			ofs.close();
		}
	}
}