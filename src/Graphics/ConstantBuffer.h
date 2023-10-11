#pragma once

struct ID3D11Device;
struct ID3D11Buffer;
struct ID3D11DeviceContext;
namespace Destiny
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer(ID3D11Device* device, unsigned int dataSize);
		~ConstantBuffer();
	public:
		void updateData(ID3D11DeviceContext* context, const void* data, unsigned int dataSize);
	public:
		ID3D11Buffer* m_pConstantBuffer;
	};
}