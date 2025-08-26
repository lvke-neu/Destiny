#pragma once
#include "Engine/Asset.h"
#include "Engine/Blob.h"
#include "GraphicsPipeline/GraphicsCommand.h"
#include "Graphics/Texture.h"
#include "ConstantBuffer.h"
#include <vector>
#include <string>

struct ID3D11ComputeShader;
struct ID3D10Blob;
namespace Destiny
{
	class ComputerEffectLoader;
	class ComputerCommand : public GraphicsCommand, public Asset
	{
		friend class ComputerEffectLoader;
	public:
		ComputerCommand();
		virtual ~ComputerCommand();
	public:
		static std::shared_ptr<ComputerEffectLoader> s_computerEffectLoader;
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		void setDebugName(const std::wstring& debugName);
		void setComputerEffectPath(const char* path);
		void setThreadGroupCount(unsigned int threadGroupCountX, unsigned int threadGroupCountY, unsigned int threadGroupCountZ);
		std::shared_ptr<Texture> getUnorderedAccessViews(int index);
		void setUnorderedAccessViews(const std::vector<std::shared_ptr<Texture>>& uavs);
		void setShaderResourceView(const char* name, std::shared_ptr<Texture> texture);
		template<typename T>
		void							setConstant(const char* name, T value);

		void setIndirectMode(bool bIndirectMode, std::shared_ptr<Texture> indirectTexture = nullptr, std::vector<unsigned int> indirectOffsets = std::vector<unsigned int>());
	private:
		ID3D11ComputeShader* m_computeShader;
		ID3D10Blob* m_csCompiledBlob;
		unsigned int m_threadGroupCountX;
		unsigned int m_threadGroupCountY;
		unsigned int m_threadGroupCountZ;

		std::vector<std::shared_ptr<Texture>> m_uavs;
		std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>> m_srvs;
		std::wstring m_debugName;

		bool m_bIndirectMode;
		std::shared_ptr<Texture>  m_indirectTexture;
		std::vector<unsigned int> m_indirectOffsets;

		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>> m_constantBuffers;
		std::unordered_map<std::string, std::string> m_variableLinkConstant;
	};

	template<typename T>
	inline void ComputerCommand::setConstant(const char* name, T value)
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