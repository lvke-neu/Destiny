#pragma once
#include "Graphics/GraphicsPipeline/GraphicsCommand.h"
#include <memory>

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class SceneAABBCalculatorCommand : public GraphicsCommand
	{
	public:
		SceneAABBCalculatorCommand();
		virtual ~SceneAABBCalculatorCommand();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		void setParam
		(	
			unsigned int numElements,
			std::shared_ptr<Texture> aabbBuffer,
			std::shared_ptr<Texture> outputBuffer,
			std::shared_ptr<ComputerCommand> calculateSceneAABBFromBVHs,
			std::shared_ptr<ComputerCommand> calculateSceneAABBFromAABBs
		);
	private:
		unsigned int m_numElements;
		std::shared_ptr<Texture> m_aabbBuffer;
		std::shared_ptr<Texture> m_outputBuffer;
		std::shared_ptr<ComputerCommand> m_calculateSceneAABBFromBVHs;
		std::shared_ptr<ComputerCommand> m_calculateSceneAABBFromAABBs;

		std::shared_ptr<Texture> m_scratchBuffers[2];
	};
}