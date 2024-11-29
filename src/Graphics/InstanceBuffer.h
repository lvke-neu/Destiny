#pragma once
#include <memory>

struct ID3D11Buffer;
namespace Destiny
{
	class Blob;
	class InstanceBuffer
	{
		friend class Mesh;
	public:
		InstanceBuffer(unsigned int byteWidth, unsigned int stride, unsigned int offset, unsigned int instanceCount);
		~InstanceBuffer();
	public:
		void updateInstanceData(std::shared_ptr<Blob> data);
	private:
		ID3D11Buffer* m_instanceBuffer;
		unsigned int  m_byteWidth;
		unsigned int  m_stride;
		unsigned int  m_offset;
		unsigned int  m_instanceCount;
	};

}