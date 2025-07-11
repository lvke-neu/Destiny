#pragma once
#include "Engine/Asset.h"
#include "GraphicsPipeline/GraphicsCommand.h"
#include "Graphics/Texture.h"
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
	private:
		ID3D11ComputeShader* m_computeShader;
		ID3D10Blob* m_csCompiledBlob;
		unsigned int m_threadGroupCountX;
		unsigned int m_threadGroupCountY;
		unsigned int m_threadGroupCountZ;

		std::vector<std::shared_ptr<Texture>> m_uavs;
		std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>> m_srvs;
		std::wstring m_debugName;
	};
}