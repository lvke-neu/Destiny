#pragma once 
#include <memory>

struct D3D11_INPUT_ELEMENT_DESC;
namespace Destiny
{
	class Blob;
	class VertexBufferFile
	{
	public:
		static bool ParseXml(const void* fileData, unsigned int length);
		static std::shared_ptr<Blob> BuildVertexBufferFile(D3D11_INPUT_ELEMENT_DESC* inputElements, unsigned int inputElementsLength, unsigned int stride, unsigned int offset, const void* vertexData, unsigned int vertexDataLength);
	};
}