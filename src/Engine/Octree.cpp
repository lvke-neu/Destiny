#include "Octree.h"

namespace Destiny
{
	Octree::Octree()
	{
		m_childs[0] = nullptr;
		m_childs[1] = nullptr;
		m_childs[2] = nullptr;
		m_childs[3] = nullptr;
		m_childs[4] = nullptr;
		m_childs[5] = nullptr;
		m_childs[6] = nullptr;
		m_childs[7] = nullptr;

		DirectX::XMFLOAT3 point = { 0.0f, 0.0f, 0.0f };
		DirectX::BoundingBox::CreateFromPoints(m_aabb, XMLoadFloat3(&point), XMLoadFloat3(&point));
	}
}