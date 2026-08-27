#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class Texture;
	class ComputerCommand;
	class CopyStructureCount;
	class GraphicsCommandList;
	class ApplyBvhComponent2 : public Component
	{
		friend class GpuBvh2BuilderComponent;
		RTTR_ENABLE(Component);
	public:
		ApplyBvhComponent2();
		virtual ~ApplyBvhComponent2() = default;
	public:
		void init(unsigned int elementCount, std::shared_ptr<Texture> outputBVH, std::shared_ptr<Texture> bitonicSortIndicesBuffer);
	public:
		virtual void onUpdate(float deltaTime) override;
	private:
		void applyBvhTopLevelCS();
		void applyBvhMiddleLevelCS();
		void applyBvhBottomLevelCS(unsigned int elementCount);
		void copyStructureCount();
	private:
		std::shared_ptr<GraphicsCommandList> m_graphicsCommandList;
		std::shared_ptr<ComputerCommand> m_applyBvhTopLevelCS;
		std::shared_ptr<ComputerCommand> m_applyBvhMiddleLevelCS;
		std::shared_ptr<ComputerCommand> m_applyBvhBottomLevelCS;
		std::shared_ptr<CopyStructureCount>	m_copyStructureCount;
		std::shared_ptr<CopyStructureCount>	m_copyStructureCount2;

		std::shared_ptr<Texture> m_outputBVH;
		std::shared_ptr<Texture> m_bitonicSortIndicesBuffer;
		std::shared_ptr<Texture> m_stack;
		std::shared_ptr<Texture> m_nextLayerBuffer;
		std::shared_ptr<Texture> m_nextLayerBuffer2;
		std::shared_ptr<Texture> m_cullElement;
		std::shared_ptr<Texture> m_indirectBuffer;
		std::shared_ptr<Texture> m_indirectBuffer2;
		std::shared_ptr<Texture> m_cullElementCount;
	};
}