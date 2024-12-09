#pragma once
#include "ConstantBuffer.h"
#include "Engine/Asset.h"
#include "Engine/Blob.h"
#include <string>
#include <unordered_map>
#include <DirectXMath.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D10Blob;
struct ID3D11ShaderReflection;
namespace Destiny
{
	class  Blob;
	class  BlobHolder;
	class  DrawParameters;
	class  ConstantBuffer;
	class  Texture;
	struct TextureDesc;
	class  SamplerState;
	struct SamplerStateDesc;
	class Renderer : public Asset
	{
	public:
		Renderer(const char* path);
		virtual ~Renderer();
	public:
		virtual void doLoad() override;
	public:
		std::shared_ptr<Blob> getInputSignatureBlob();
		void fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters);
	public:
		template<typename T>
		void setConstant(const char* name, T value);
		void setShaderResource(const char* name, std::shared_ptr<Texture> texture);
		void setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState);
	private:
		bool createVertexShader();
		bool createPixelShader();
		bool createGeometryShader();
		void collectReflectionInfo(ID3D10Blob* compiledBlob, short flag);
		void collectReflectionConstantInfo(ID3D11ShaderReflection* shaderReflection, short flag);
		void collectReflectionTextureInfo(ID3D11ShaderReflection* shaderReflection, short flag);
		void collectReflectionSamplerStateInfo(ID3D11ShaderReflection* shaderReflection, short flag);
	private:
		ID3D11VertexShader*				m_vertexShader;
		ID3D11PixelShader*				m_pixelShader;
		ID3D11GeometryShader*			m_geometryShader;
		std::shared_ptr<Blob>			m_inputSignatureBlob;
		std::shared_ptr<BlobHolder>		m_blobHolder;
		ID3D10Blob*						m_vsCompiledBlob;
		ID3D10Blob*						m_psCompiledBlob;
		ID3D10Blob*						m_gsCompiledBlob;
		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>> m_constantBuffers;
		std::unordered_map<std::string, std::string> m_variableLinkConstant;
		std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>> m_textures;
		std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>> m_samplerStates;
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