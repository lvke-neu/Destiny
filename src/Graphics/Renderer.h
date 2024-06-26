#pragma once
#include "ConstantBuffer.h"
#include "Engine/Asset.h"
#include "Engine/Blob.h"
#include <string>
#include <unordered_map>
#include <DirectXMath.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D10Blob;
namespace Destiny
{
	class Blob;
	class BlobHolder;
	class RenderParameters;
	class ConstantBuffer;
	class Renderer : public Asset
	{
	public:
		Renderer(const char* path);
		virtual ~Renderer();
	public:
		virtual void doLoad() override;
	public:
		std::shared_ptr<Blob> getInputSignatureBlob();
		void fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters);
	public:
		template<typename T>
		void setConstant(const char* name, T value);
	private:
		bool createVertexShader();
		bool createPixelShader();
		void collectReflectionInfo(ID3D10Blob* compiledBlob, short flag);
	private:
		ID3D11VertexShader*				m_vertexShader;
		ID3D11PixelShader*				m_pixelShader;
		std::shared_ptr<Blob>			m_inputSignatureBlob;
		std::shared_ptr<BlobHolder>		m_blobHolder;
		ID3D10Blob*						m_vsCompiledBlob;
		ID3D10Blob*						m_psCompiledBlob;
		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>> m_constantBuffers;
		std::unordered_map<std::string, std::string> m_variableLinkConstant;
	};

	inline std::shared_ptr<Blob> Renderer::getInputSignatureBlob()
	{
		return m_inputSignatureBlob;
	}

	template<typename T>
	void Renderer::setConstant(const char* name, T value)
	{
		auto iter = m_variableLinkConstant.find(name);
		if (iter == m_variableLinkConstant.end())
		{
			return;
		}

		if (m_constantBuffers[iter->second])
		{
			std::shared_ptr<Blob> blob = std::make_shared<Blob>(sizeof(value));
			blob->copyfrom(&value, sizeof(value));
			m_constantBuffers[iter->second]->setVariable(name, blob);
		}
	}
}