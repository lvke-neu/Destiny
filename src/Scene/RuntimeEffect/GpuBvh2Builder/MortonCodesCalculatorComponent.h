#pragma once
#include "Engine/Component.h"
#include "StructDefine.h"

namespace Destiny
{
	class ComputerCommand;
	class GraphicsCommandList;
	class Texture;
	class MortonCodesCalculatorComponent : public Component
	{
		friend class GpuBvh2BuilderComponent;
		RTTR_ENABLE(Component);
	public:
		MortonCodesCalculatorComponent();
		virtual ~MortonCodesCalculatorComponent() = default;
	public:
		void init(const std::vector<AABB>& aabbs, std::shared_ptr<Texture> sceneAABB);
	private:
		void calcuateMortonCodesForAABBsCS();
	private:
		std::shared_ptr<ComputerCommand> m_calcuateMortonCodesForAABBs;
		std::shared_ptr<GraphicsCommandList>	m_graphicsCommandList;

		std::shared_ptr<Texture> m_outputIndicesBuffer;
		std::shared_ptr<Texture> m_outputMortonCodesBuffer;
		std::shared_ptr<Texture> m_sceneAABB;
		std::shared_ptr<Texture> m_inputBuffer;
	};
}