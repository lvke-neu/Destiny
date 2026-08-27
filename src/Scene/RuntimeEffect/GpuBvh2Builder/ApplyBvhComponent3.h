#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class Texture;
	class ComputerCommand;
	class CopyStructureCount;
	class GraphicsCommandList;
	class ApplyBvhComponent3 : public Component
	{
		RTTR_ENABLE(Component);
	public:
		ApplyBvhComponent3();
		virtual ~ApplyBvhComponent3() = default;
	public:
		void init(unsigned int elementCount, std::shared_ptr<Texture> clustersSmallers, 
		std::shared_ptr<Texture> aabbuffer,
		std::shared_ptr<Texture> clusterOffsets,
		std::shared_ptr<Texture> clusterElements);
	public:
		virtual void onUpdate(float deltaTime) override;
	private:
		std::shared_ptr<GraphicsCommandList> m_graphicsCommandList;

		std::shared_ptr<Texture> m_indirectBuffer;
		std::shared_ptr<Texture> m_indirectBuffer2;

		std::shared_ptr<Texture> m_clustersSmallers;
		std::shared_ptr<Texture> m_commitClusters;
		std::shared_ptr<Texture> m_aabbuffer;
		std::shared_ptr<Texture> m_clusterOffsets;
		std::shared_ptr<Texture> m_clusterElements;
		std::shared_ptr<Texture> m_commitElements;

		std::shared_ptr<CopyStructureCount> m_copyStructureCount;
		std::shared_ptr<CopyStructureCount> m_copyStructureCount2;
		std::shared_ptr<ComputerCommand> m_cullClusters;
		std::shared_ptr<ComputerCommand> m_cullElements;
	};
}