#pragma once
#include "OctreeNode.h"

namespace Destiny
{
	class Visual;
	class Octree : public OctreeNode
	{
	public:
		Octree(float sceneRadius, unsigned int octreeMaxDepth, unsigned int octreeMaxCapacity);
	public:
		void addVisual(std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldAABB);
	private:
		float m_sceneRadius;
		unsigned int m_octreeMaxDepth;
		unsigned int m_octreeMaxCapacity;
	};
}