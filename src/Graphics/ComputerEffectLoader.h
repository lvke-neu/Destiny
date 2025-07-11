#pragma once
#include "Engine/AssetLoader.h"

struct ID3D10Blob;
struct ID3D11ShaderReflection;
namespace Destiny
{
	class ComputerCommand;
	class ComputerEffectLoader : public AssetLoader
	{
	public:
		~ComputerEffectLoader() = default;
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
	private:
		void							collectReflectionInfo(ID3D10Blob* compiledBlob, std::shared_ptr<ComputerCommand> computerCommand);
		//void							collectReflectionConstantInfo(ID3D11ShaderReflection* shaderReflection);
		void							collectReflectionTextureInfo(ID3D11ShaderReflection* shaderReflection, std::shared_ptr<ComputerCommand> computerCommand);
		//void							collectReflectionSamplerStateInfo(ID3D11ShaderReflection* shaderReflection);
	};
}

