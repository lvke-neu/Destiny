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
struct ID3D11HullShader;
struct ID3D11DomainShader;
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

		template<typename T>
		static void	SetConstant(const char* name, T value);
		static void SetConstant(const char* name, std::shared_ptr<Blob> blob);
		static void SetShaderResource(const char* name, std::shared_ptr<Texture> texture);
		static void SetSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState);
		static std::unordered_map<std::string, std::shared_ptr<Blob>> s_cache_constant;
		static std::unordered_map<std::string, std::shared_ptr<Texture>> s_cache_shaderResource;
		static std::unordered_map<std::string, std::shared_ptr<SamplerState>> s_cache_samplerSate;
		static void ClearCache();
	public:
		std::shared_ptr<Blob>			getInputSignatureBlob();
		void							fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters);
		void							fillConstantBuffers(std::unordered_map<std::string, std::string>& variableLinkConstant, std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& constantBuffers);
		void							modifyConstantBuffersByDifference(std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& constantBuffers);
		void							fillTextures(std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>& textures);
		void							modifyTexturesByDifferenece(std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>& textures);
		void							fillSamplerStates(std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>& samplerStates);
		void							modifySamplerStatesByDifferenece(std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>& samplerStates);
		template<typename T>
		void							setConstant(const char* name, T value);
		void							setConstant(const char* name, std::shared_ptr<Blob> blob);
		void							setShaderResource(const char* name, std::shared_ptr<Texture> texture);
		void							setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState);
	public:
		std::string						getPath();
		std::shared_ptr<BlobHolder>		getBlobHolder();
	private:
		bool							createVertexShader();
		bool							createPixelShader();
		bool							createGeometryShader();
		bool							createHullShader();
		bool							createDomainShader();
		void							collectReflectionInfo(ID3D10Blob* compiledBlob, short flag);
		void							collectReflectionConstantInfo(ID3D11ShaderReflection* shaderReflection, short flag);
		void							collectReflectionTextureInfo(ID3D11ShaderReflection* shaderReflection, short flag);
		void							collectReflectionSamplerStateInfo(ID3D11ShaderReflection* shaderReflection, short flag);
	private:
		void*							m_vertexShader;
		void*							m_pixelShader;
		void*							m_geometryShader;
		void*							m_hullShader;
		void*							m_domainShader;
		std::shared_ptr<Blob>			m_inputSignatureBlob;
		std::shared_ptr<BlobHolder>		m_blobHolder;
		ID3D10Blob*						m_vsCompiledBlob;
		ID3D10Blob*						m_psCompiledBlob;
		ID3D10Blob*						m_gsCompiledBlob;
		ID3D10Blob*						m_hsCompiledBlob;
		ID3D10Blob*						m_dsCompiledBlob;
		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>> m_constantBuffers;
		std::unordered_map<std::string, std::string> m_variableLinkConstant;
		std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>> m_textures;
		std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>> m_samplerStates;
		std::unordered_set<std::string>	m_changedConstantBufferNames;
		std::unordered_set<std::string>	m_changedTextureNames;
		std::unordered_set<std::string>	m_changedSamplerStateNames;
	};

	inline std::shared_ptr<Blob> Renderer::getInputSignatureBlob()
	{
		return m_inputSignatureBlob;
	}

	inline std::shared_ptr<BlobHolder> Renderer::getBlobHolder()
	{
		return m_blobHolder;
	}

	template<typename T>
	inline void	Renderer::SetConstant(const char* name, T value)
	{
		std::shared_ptr<Blob> blob = std::make_shared<Blob>(sizeof(value));
		blob->copyfrom(&value, sizeof(value));
		s_cache_constant[name] = blob;
		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setConstant(name, value);
		}
	}

	template<typename T>
	inline void Renderer::setConstant(const char* name, T value)
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
			m_changedConstantBufferNames.insert(name);
		}
	}

	inline void Renderer::setConstant(const char* name, std::shared_ptr<Blob> blob)
	{
		auto iter = m_variableLinkConstant.find(name);
		if (iter == m_variableLinkConstant.end())
		{
			return;
		}

		if (m_constantBuffers[iter->second])
		{
			m_constantBuffers[iter->second]->setVariable(name, blob);
			m_changedConstantBufferNames.insert(name);
		}
	}
}