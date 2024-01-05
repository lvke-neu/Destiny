#pragma once
#include <DirectXCollision.h>
#include <memory>

namespace Destiny
{
	class Octree
	{
	public:
		Octree();
	public:
		DirectX::BoundingBox m_aabb;
		std::shared_ptr<Octree> m_childs[8];
	};
}