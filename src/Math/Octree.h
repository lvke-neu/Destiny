#pragma
#include "OctreeNode.h"

namespace Destiny
{
    class Octree {
    public:
        Octree(const DirectX::BoundingBox& worldBounds);
        ~Octree();
    public:
        void insert(std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldBox);
        void getVisibleObjects(const DirectX::BoundingFrustum& frustum, std::vector<std::shared_ptr<Visual>>& outObjects) const;
    private:
        std::unique_ptr<OctreeNode> root;
    };
}