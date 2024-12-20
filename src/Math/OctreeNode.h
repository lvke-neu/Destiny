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
		std::shared_ptr<OctreeNode>				childs[8];
		int										level;
		std::vector<std::shared_ptr<Visual>>	visual;
	};
}