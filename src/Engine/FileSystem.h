#pragma once
#include <memory>
#include <string>

namespace Destiny
{
	class Blob;
	class FileSystem
	{
	public:
		static std::shared_ptr<Blob> ReadBlob(const char* path);
		static void WriteBlob(const char* path, std::shared_ptr<Blob> blob);

		static std::string ReplaceFileSuffix(const std::string& filePath, const std::string& newSuffix);
		static std::string ReplaceFileSuffixAddFileName(const std::string& filePath, const std::string& fileName, const std::string& newSuffix);
	};
}