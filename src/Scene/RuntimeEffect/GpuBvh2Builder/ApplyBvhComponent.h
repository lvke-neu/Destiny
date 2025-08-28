#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class Texture;
	class ComputerCommand;
	class CopyStructureCount;
	class GraphicsCommandList;
	class ApplyBvhComponent : public Component
	{
		friend class GpuBvh2BuilderComponent;
		RTTR_ENABLE(Component);
	public:
		ApplyBvhComponent();
		virtual ~ApplyBvhComponent() = default;
	public:
		void init(unsigned int elementCount, std::shared_ptr<Texture> outputBVH, std::shared_ptr<Texture> hierarchyBuffer, std::shared_ptr<Texture> bitonicSortIndicesBuffer);
	public:
		virtual void onUpdate(float deltaTime) override;
		virtual void onLeaveScene() override;
	private:
		void applyBvhCS(unsigned int elementCount);
		void copyStructureCount();
	private:
		std::shared_ptr<GraphicsCommandList> m_graphicsCommandList;
		std::shared_ptr<ComputerCommand> m_applyBvhCS;
		std::shared_ptr<CopyStructureCount> m_copyStructureCount;

		std::shared_ptr<Texture> m_outputBVH;
		std::shared_ptr<Texture> m_hierarchyBuffer;
		std::shared_ptr<Texture> m_bitonicSortIndicesBuffer;
		std::shared_ptr<Texture> m_stack;
		std::shared_ptr<Texture> m_cullElement;
		std::shared_ptr<Texture> m_cullElementCount;
	};
}