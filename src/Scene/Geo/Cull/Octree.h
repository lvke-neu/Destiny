#pragma once
#include <memory>
#include <DirectXCollision.h>

namespace Destiny
{
	struct OctreeNode;
	class Visual;
	class Octree
	{
	public:
		Octree(float sceneRadius, int octreeDepth);
	public:
		void addVisual(std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldAABB);
		void onCull(const DirectX::BoundingFrustum& frustum);
	private:
		void addVisual(std::shared_ptr<OctreeNode> octreeNode, std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldAABB);
		void onCull(std::shared_ptr<OctreeNode> OctreeNode, const DirectX::BoundingFrustum& frustum);
	private:
		void constructOctree(float sceneRadius);
		void constructOctreeNodeChilds(std::shared_ptr<OctreeNode> octreeNode);
	private:
		std::shared_ptr<OctreeNode> m_rootOctreeNode;
		int m_octreeDepth;
	};
}