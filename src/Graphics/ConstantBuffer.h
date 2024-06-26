#pragma once
#include <string>
#include <memory>
#include <unordered_map>

struct ID3D11Buffer;
struct ID3D11DeviceContext;
namespace Destiny
{
	enum class ConstantBufferBindFlag
	{
		BindVS,
		BindPS
	};

	struct ConstantBufferVariable
	{
		unsigned int offset;
		unsigned int size;
	};

	class Blob;
	class ConstantBuffer
	{
	public:
		ConstantBuffer(unsigned int startSlot, unsigned int byteWidth);
		~ConstantBuffer();
	public:
		void addVariable(const std::string& name, ConstantBufferVariable variable);
		void setVariable(const std::string& name, std::shared_ptr<Blob> data);
		void setConstantBufferBindFlag(ConstantBufferBindFlag bindFlag, bool value);
		void bind();
	private:
		unsigned int m_startSlot;
		unsigned int m_byteWidth;
		std::unordered_map<std::string, ConstantBufferVariable> m_variables;
		std::unordered_map<ConstantBufferBindFlag, bool> m_constantBufferBindFlag;
		std::shared_ptr<Blob> m_backData;
		ID3D11Buffer* m_constantBuffer;
	};

}