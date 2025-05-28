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

	std::string FileSystem::ReplaceFileSuffix(const std::string& filePath, const std::string& newSuffix)
	{
        size_t dotPos = filePath.find_last_of('.');

        size_t pathSepPos = filePath.find_last_of("/\\");

        if (dotPos != std::string::npos && (pathSepPos == std::string::npos || dotPos > pathSepPos))
        {
            if (!newSuffix.empty() && newSuffix[0] == '.')
            {
                return filePath.substr(0, dotPos) + newSuffix;
            }
            else
            {
                return filePath.substr(0, dotPos) + '.' + newSuffix;
            }
        }
        else
        {
            if (!newSuffix.empty() && newSuffix[0] == '.')
            {
                return filePath + newSuffix;
            }
            else
            {
                return filePath + '.' + newSuffix;
            }
        }
	}

	std::string FileSystem::ReplaceFileSuffixAddFileName(const std::string& filePath, const std::string& fileName, const std::string& newSuffix)
	{
        size_t dotPos = filePath.find_last_of('.');

        size_t pathSepPos = filePath.find_last_of("/\\");

        if (dotPos != std::string::npos && (pathSepPos == std::string::npos || dotPos > pathSepPos))
        {
            if (!newSuffix.empty() && newSuffix[0] == '.')
            {
                return filePath.substr(0, dotPos) + fileName + newSuffix;
            }
            else
            {
                return filePath.substr(0, dotPos) + fileName + '.' + newSuffix;
            }
        }
        else
        {
            if (!newSuffix.empty() && newSuffix[0] == '.')
            {
                return filePath + fileName +newSuffix;
            }
            else
            {
                return filePath + fileName + '.' + newSuffix;
            }
        }
	}
}