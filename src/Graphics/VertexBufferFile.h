#pragma once 

namespace Destiny
{
	class VertexBufferFile
	{
	public:
		static bool ParseXml(const void* fileData, unsigned int length);
	};
}