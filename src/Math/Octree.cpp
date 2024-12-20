#include "Octree.h"
#include "OctreeNode.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	Octree::Octree(float sceneRadius, int octreeDepth) :
		m_rootOctreeNode(nullptr),
		m_octreeDepth(octreeDepth)
	{
		constructOctree(sceneRadius);
	}

	void Octree::addVisual(std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldAABB)
	{
		if (!visual)
		{
			return;
		}

		addVisual(m_rootOctreeNode, visual, worldAABB);
	}

	void Octree::onCull(const DirectX::BoundingFrustum& frustum)
	{
		onCull(m_rootOctreeNode, frustum);
	}

	void Octree::addVisual(std::shared_ptr<OctreeNode> octreeNode, std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldAABB)
	{
		if (!octreeNode || !visual)
		{
			return;
		}

		if (octreeNode->level == m_octreeDepth)
		{
			if (octreeNode->box.Intersects(worldAABB))
			{
				octreeNode->visual.push_back(visual);
				return;
			}
		}

		if (octreeNode->box.Intersects(worldAABB))
		{
			for (const auto& childOctreeNode : octreeNode->childs)
			{
				addVisual(childOctreeNode, visual, worldAABB);
			}
		}
	}

	void Octree::onCull(std::shared_ptr<OctreeNode> octreeNode, const DirectX::BoundingFrustum& frustum)
	{
		if (!octreeNode)
		{
			return;
		}

		if (octreeNode->level == m_octreeDepth)
		{
			for (const auto& visual : octreeNode->visual)
			{
				Engine::GetInstance()->getGraphicsSystem()->commitVisual(visual);
			}
			return;
		}

		for (const auto& childOctreeNode : octreeNode->childs)
		{
			onCull(childOctreeNode, frustum);
		}
	}

	void Octree::constructOctree(float sceneRadius)
	{
		m_rootOctreeNode = std::make_shared<OctreeNode>();
		m_rootOctreeNode->box = { { 0.0f, 0.0f, 0.0f }, { sceneRadius, sceneRadius, sceneRadius } };
		m_rootOctreeNode->level = 0;
		constructOctreeNodeChilds(m_rootOctreeNode);
	}

	void Octree::constructOctreeNodeChilds(std::shared_ptr<OctreeNode> octreeNode)
	{
		if (!octreeNode || octreeNode->level >= m_octreeDepth)
		{
			return;
		}

		auto center = octreeNode->box.Center;
		auto extent = octreeNode->box.Extents;

		auto childOctreeNode0 = std::make_shared<OctreeNode>();
		auto childOctreeNode1 = std::make_shared<OctreeNode>();
		auto childOctreeNode2 = std::make_shared<OctreeNode>();
		auto childOctreeNode3 = std::make_shared<OctreeNode>();
		auto childOctreeNode4 = std::make_shared<OctreeNode>();
		auto childOctreeNode5 = std::make_shared<OctreeNode>();
		auto childOctreeNode6 = std::make_shared<OctreeNode>();
		auto childOctreeNode7 = std::make_shared<OctreeNode>();

		childOctreeNode0->box = { { center.x - extent.x / 2.0f, center.y + extent.y / 2.0f, center.z + extent.z / 2.0f }, { extent.x / 2.0f, extent.y / 2.0f, extent.z / 2.0f } };
		childOctreeNode0->level = octreeNode->level + 1;

		childOctreeNode1->box = { { center.x + extent.x / 2.0f, center.y + extent.y / 2.0f, center.z + extent.z / 2.0f }, { extent.x / 2.0f, extent.y / 2.0f, extent.z / 2.0f } };
		childOctreeNode1->level = octreeNode->level + 1;

		childOctreeNode2->box = { { center.x - extent.x / 2.0f, center.y + extent.y / 2.0f, center.z - extent.z / 2.0f }, { extent.x / 2.0f, extent.y / 2.0f, extent.z / 2.0f } };
		childOctreeNode2->level = octreeNode->level + 1;

		childOctreeNode3->box = { { center.x + extent.x / 2.0f, center.y + extent.y / 2.0f, center.z - extent.z / 2.0f }, { extent.x / 2.0f, extent.y / 2.0f, extent.z / 2.0f } };
		childOctreeNode3->level = octreeNode->level + 1;

		childOctreeNode4->box = { { center.x - extent.x / 2.0f, center.y - extent.y / 2.0f, center.z + extent.z / 2.0f }, { extent.x / 2.0f, extent.y / 2.0f, extent.z / 2.0f } };
		childOctreeNode4->level = octreeNode->level + 1;

		childOctreeNode5->box = { { center.x + extent.x / 2.0f, center.y - extent.y / 2.0f, center.z + extent.z / 2.0f }, { extent.x / 2.0f, extent.y / 2.0f, extent.z / 2.0f } };
		childOctreeNode5->level = octreeNode->level + 1;

		childOctreeNode6->box = { { center.x - extent.x / 2.0f, center.y - extent.y / 2.0f, center.z - extent.z / 2.0f }, { extent.x / 2.0f, extent.y / 2.0f, extent.z / 2.0f } };
		childOctreeNode6->level = octreeNode->level + 1;

		childOctreeNode7->box = { { center.x + extent.x / 2.0f, center.y - extent.y / 2.0f, center.z - extent.z / 2.0f }, { extent.x / 2.0f, extent.y / 2.0f, extent.z / 2.0f } };
		childOctreeNode7->level = octreeNode->level + 1;

		octreeNode->childs[0] = childOctreeNode0;
		octreeNode->childs[1] = childOctreeNode1;
		octreeNode->childs[2] = childOctreeNode2;
		octreeNode->childs[3] = childOctreeNode3;
		octreeNode->childs[4] = childOctreeNode4;
		octreeNode->childs[5] = childOctreeNode5;
		octreeNode->childs[6] = childOctreeNode6;
		octreeNode->childs[7] = childOctreeNode7;

		constructOctreeNodeChilds(childOctreeNode0);
		constructOctreeNodeChilds(childOctreeNode1);
		constructOctreeNodeChilds(childOctreeNode2);
		constructOctreeNodeChilds(childOctreeNode3);
		constructOctreeNodeChilds(childOctreeNode4);
		constructOctreeNodeChilds(childOctreeNode5);
		constructOctreeNodeChilds(childOctreeNode6);
		constructOctreeNodeChilds(childOctreeNode7);
	}					   				   		
}