#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class SceneAABBCalculatorCommand;
	class ComputerCommand;
	class GraphicsCommandList;
	class Texture;
	class SceneAABBCalculatorComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		SceneAABBCalculatorComponent();
		virtual ~SceneAABBCalculatorComponent() = default;
	private:
		void generateFakeData();
		void calculateSceneAABBFromBVHsCS();
		void calculateSceneAABBFromAABBsCS();
	private:
		std::shared_ptr<ComputerCommand> m_calculateSceneAABBFromBVHs;
		std::shared_ptr<ComputerCommand> m_calculateSceneAABBFromAABBs;
		std::shared_ptr<SceneAABBCalculatorCommand> m_sceneAABBCalculatorCommand;
		std::shared_ptr<GraphicsCommandList>	m_graphicsCommandList;

		std::shared_ptr<Texture> m_aabbBuffer;
		std::shared_ptr<Texture> m_outputBuffer;

		unsigned int m_numElements;
	};
}