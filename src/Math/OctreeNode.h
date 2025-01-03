#pragma once
#include <memory>
#include <vector>
#include <DirectXCollision.h>

namespace Destiny
{
	class Visual;
	struct OctreeNode
	{
		DirectX::BoundingBox					box;
		unsigned int							depth;
		std::shared_ptr<OctreeNode>				childs[8];
		std::vector<std::shared_ptr<Visual>>	visuals;

		bool isLeafNode()
		{
			return visuals.size() ? false : true;
		}
	};
}