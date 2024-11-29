#pragma once

struct ID3D11Buffer;
namespace Destiny
{
	class Blob;
	class InstanceBuffer
	{
	public:
		InstanceBuffer(unsigned int byteWidth);
		~InstanceBuffer();
	private:
		ID3D11Buffer* m_instanceBuffer;
	};

}