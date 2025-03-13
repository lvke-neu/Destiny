#pragma once
#include <memory>

namespace Destiny
{
	class Blob;
	class FileSystem
	{
	public:
		static std::shared_ptr<Blob> ReadBlob(const char* path);
		static void WriteBlob(const char* path, std::shared_ptr<Blob> blob);
	};
}