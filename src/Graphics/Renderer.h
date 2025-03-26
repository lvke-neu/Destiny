#pragma once
#include "ConstantBuffer.h"
#include "Engine/Asset.h"
#include "Engine/Blob.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <DirectXMath.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D10Blob;
struct ID3D11ShaderReflection;
namespace Destiny
{
	class  BlobHolder;
	class  DrawParameters;
	class  Texture;
	class  SamplerState;
	struct TextureDesc;
	struct SamplerStateDesc;
	class Renderer : public Asset
	{
	public:
		Renderer(const char* path);
		virtual ~Renderer();
	public:
		virtual void doLoad() override;
		virtual void doReload() override;
	public:
		static std::shared_ptr<Renderer> Create(const char* path);
		static std::unordered_map<std::string, std::shared_ptr<Renderer>> s_cache;
	public:
		std::shared_ptr<Blob>			getInputSignatureBlob();
		void							fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters);
		void							fillConstantBuffers(std::unordered_map<std::string, std::string>& variableLinkConstant, std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& constantBuffers);
		void							fillTextures(std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>& textures);
		void							fillSamplerStates(std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>& samplerStates);
	public:
		std::string						getPath();
		std::shared_ptr<BlobHolder>		getBlobHolder();
	private:
		bool							createVertexShader();
		bool							createPixelShader();
		bool							createGeometryShader();
		void							collectReflectionInfo(ID3D10Blob* compiledBlob, short flag);
		void							collectReflectionConstantInfo(ID3D11ShaderReflection* shaderReflection, short flag);
		void							collectReflectionTextureInfo(ID3D11ShaderReflection* shaderReflection, short flag);
		void							collectReflectionSamplerStateInfo(ID3D11ShaderReflection* shaderReflection, short flag);
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
		std::unordered_map<std::string, std::shared_ptr<TextureDesc>> m_textures;
		std::unordered_map<std::string, std::shared_ptr<SamplerStateDesc>> m_samplerStates;
	};

	inline std::shared_ptr<Blob> Renderer::getInputSignatureBlob()
	{
		return m_inputSignatureBlob;
	}

	inline std::shared_ptr<BlobHolder> Renderer::getBlobHolder()
	{
		return m_blobHolder;
	}
}