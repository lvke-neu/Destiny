#pragma once
#include <memory>
#include <vector>
#include <DirectXCollision.h>
#include <array>

namespace Destiny
{
    class Visual;
    struct Visual_WorldBox
    {
        std::shared_ptr<Visual> visual = nullptr;
        DirectX::BoundingBox worldBox;
    };
    class OctreeNode {
    public:
        static const int MAX_OBJECTS = 5;
        static const int MAX_DEPTH = 8;  

        OctreeNode(const DirectX::BoundingBox& bounds, int depth = 0);
        ~OctreeNode();
    public:
        bool insert(std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldBox);
        void split();
        void getVisibleObjects(const DirectX::BoundingFrustum& frustum, std::vector<std::shared_ptr<Visual>>& outVisuals) const;

    private:
        DirectX::BoundingBox bounds;
        int depth;                       
        std::vector<Visual_WorldBox> visuals;
        std::array<std::unique_ptr<OctreeNode>, 8> children;
        bool isLeaf;
    };
}