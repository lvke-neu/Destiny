#include "Octree.h"

namespace Destiny
{
    Octree::Octree(const DirectX::BoundingBox& worldBounds)
    {
        root = std::make_unique<OctreeNode>(worldBounds);
    }

    Octree::~Octree() 
    {
    }

    void Octree::insert(std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldBox) 
    {
        root->insert(visual, worldBox);
    }

   void Octree::getVisibleObjects(const DirectX::BoundingFrustum& frustum, std::vector<std::shared_ptr<Visual>>& outObjects) const
    {
        root->getVisibleObjects(frustum, outObjects);
    }
}