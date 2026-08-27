#include "OctreeNode.h"
#include "Graphics/Visual.h"

namespace Destiny
{
    OctreeNode::OctreeNode(const DirectX::BoundingBox& bounds, int depth) : 
        bounds(bounds), 
        depth(depth), 
        isLeaf(true) 
    {
        //children.fill(nullptr);
    }

    OctreeNode::~OctreeNode() 
    {

    }

    bool OctreeNode::insert(std::shared_ptr<Visual> visual, const DirectX::BoundingBox& worldBox)
    {
        if (!visual)
        {
            return false;
        }

        auto mesh = visual->getMesh();
        if (!mesh)
        {
            return false;
        }
      
        if (!bounds.Intersects(worldBox)) 
        {
            return false;
        }

        if (isLeaf && (visuals.size() < MAX_OBJECTS || depth >= MAX_DEPTH))
        {
            visuals.push_back({ visual, worldBox });
            return true;
        }

        if (isLeaf) 
        {
            split();
            isLeaf = false;

            std::vector<Visual_WorldBox> temp = std::move(visuals);
            for (auto& obj : temp)
            {
                insert(obj.visual, obj.worldBox);
            }
        }

        for (auto& child : children)
        {
            if (child->insert(visual, worldBox))
            {
                return true;
            }
        }

        visuals.push_back({ visual, worldBox });
        return true;
    }

    void OctreeNode::split() 
    {
       auto boundsCenter = bounds.Center;
       DirectX::XMFLOAT3 boundsMax =
       {
           bounds.Extents.x - bounds.Center.x,
           bounds.Extents.y - bounds.Center.y,
           bounds.Extents.z - bounds.Center.z 
       };
       DirectX::XMFLOAT3 boundsMin =
       {
          bounds.Center.x - bounds.Extents.x,
          bounds.Center.y - bounds.Extents.y,
          bounds.Center.z - bounds.Extents.z
       };
      
       DirectX::XMFLOAT3 box0Min = boundsMin;
       DirectX::XMFLOAT3 box0Max = boundsCenter;
       DirectX::XMFLOAT3 box0Center = { box0Max.x - box0Min.x,box0Max.y - box0Min.y,box0Max.z - box0Min.z };
       DirectX::XMFLOAT3 box0Extents = { box0Max.x - box0Min.x,box0Max.y - box0Min.y,box0Max.z - box0Min.z };

       DirectX::XMFLOAT3 box1Min = { boundsCenter.x, boundsMin.y, boundsMin.z };
       DirectX::XMFLOAT3 box1Max = { boundsMax.x,boundsCenter.y, boundsCenter.z };
       DirectX::XMFLOAT3 box1Center = {  box1Max.x - box1Min.x,box1Max.y - box1Min.y,box1Max.z - box1Min.z };
       DirectX::XMFLOAT3 box1Extents = { box1Max.x - box1Min.x,box1Max.y - box1Min.y,box1Max.z - box1Min.z };

       DirectX::XMFLOAT3 box2Min = { boundsCenter.x, boundsCenter.y, boundsMin.z };
       DirectX::XMFLOAT3 box2Max = { boundsMax.x,boundsMax.y, boundsCenter.z };
       DirectX::XMFLOAT3 box2Center = {  box2Max.x - box2Min.x,box2Max.y - box2Min.y,box2Max.z - box2Min.z };
       DirectX::XMFLOAT3 box2Extents = { box2Max.x - box2Min.x,box2Max.y - box2Min.y,box2Max.z - box2Min.z };

       DirectX::XMFLOAT3 box3Min = { boundsMin.x, boundsCenter.y, boundsMin.z };
       DirectX::XMFLOAT3 box3Max = { boundsCenter.x,boundsMax.y, boundsCenter.z };
       DirectX::XMFLOAT3 box3Center = {  box3Max.x - box3Min.x,box3Max.y - box3Min.y,box3Max.z - box3Min.z };
       DirectX::XMFLOAT3 box3Extents = { box3Max.x - box3Min.x,box3Max.y - box3Min.y,box3Max.z - box3Min.z };

       DirectX::XMFLOAT3 box4Min = { boundsMin.x, boundsMin.y, boundsCenter.z };
       DirectX::XMFLOAT3 box4Max = { boundsCenter.x,boundsCenter.y, boundsMax.z };
       DirectX::XMFLOAT3 box4Center = {  box4Max.x - box4Min.x,box4Max.y - box4Min.y,box4Max.z - box4Min.z };
       DirectX::XMFLOAT3 box4Extents = { box4Max.x - box4Min.x,box4Max.y - box4Min.y,box4Max.z - box4Min.z };

       DirectX::XMFLOAT3 box5Min = { boundsCenter.x, boundsMin.y, boundsCenter.z };
       DirectX::XMFLOAT3 box5Max = { boundsMax.x,boundsCenter.y, boundsMax.z };
       DirectX::XMFLOAT3 box5Center = { box5Max.x - box5Min.x,box5Max.y - box5Min.y,box5Max.z - box5Min.z };
       DirectX::XMFLOAT3 box5Extents = { box5Max.x - box5Min.x,box5Max.y - box5Min.y,box5Max.z - box5Min.z };


       DirectX::XMFLOAT3 box6Min = { boundsCenter.x, boundsCenter.y, boundsCenter.z };
       DirectX::XMFLOAT3 box6Max = { boundsMax.x,boundsMax.y, boundsMax.z };
       DirectX::XMFLOAT3 box6Center = { box6Max.x - box6Min.x,box6Max.y - box6Min.y,box6Max.z - box6Min.z };
       DirectX::XMFLOAT3 box6Extents = { box6Max.x - box6Min.x,box6Max.y - box6Min.y,box6Max.z - box6Min.z };

       DirectX::XMFLOAT3 box7Min = { boundsMin.x, boundsCenter.y, boundsCenter.z };
       DirectX::XMFLOAT3 box7Max = { boundsCenter.x,boundsMax.y, boundsMax.z };
       DirectX::XMFLOAT3 box7Center = { box7Max.x - box7Min.x,box7Max.y - box7Min.y,box7Max.z - box7Min.z };
       DirectX::XMFLOAT3 box7Extents = { box7Max.x - box7Min.x,box7Max.y - box7Min.y,box7Max.z - box7Min.z };

       children[0] = std::make_unique<OctreeNode>(DirectX::BoundingBox( box0Center, box0Extents), depth + 1);
       children[1] = std::make_unique<OctreeNode>(DirectX::BoundingBox(box1Center, box1Extents),depth + 1);
       children[2] = std::make_unique<OctreeNode>(DirectX::BoundingBox(box2Center, box2Extents),depth + 1);
       children[3] = std::make_unique<OctreeNode>(DirectX::BoundingBox(box3Center, box3Extents),depth + 1);
       children[4] = std::make_unique<OctreeNode>(DirectX::BoundingBox(box4Center, box4Extents),depth + 1);
       children[5] = std::make_unique<OctreeNode>(DirectX::BoundingBox(box5Center, box5Extents),depth + 1);
       children[6] = std::make_unique<OctreeNode>(DirectX::BoundingBox(box6Center, box6Extents),depth + 1);
       children[7] = std::make_unique<OctreeNode>(DirectX::BoundingBox(box7Center, box7Extents),depth + 1);
    }
    
    void OctreeNode::getVisibleObjects(const DirectX::BoundingFrustum& frustum,
        std::vector<std::shared_ptr<Visual>>& outObjects) const
    {

        if (!frustum.Contains(bounds)) 
        {
            return;
        }

        for (const auto& object : visuals) {
            if (frustum.Contains(object.worldBox)) {
                outObjects.push_back(object.visual);
            }
        }
        if (!isLeaf) {
            for (const auto& child : children) {
                child->getVisibleObjects(frustum, outObjects);
            }
        }
    }
}