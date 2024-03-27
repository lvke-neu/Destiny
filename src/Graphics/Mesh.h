#pragma once
#include "Engine/Asset.h"
#include <DirectXCollision.h>

namespace Destiny
{
	class VertexBuffer;
	class IndexBuffer;
	class Mesh : public Asset
	{
	public:
		enum class DrawMethod
		{
			Undefined,
			Draw,
			DrawIndexed
		};
		enum class PrimitiveTopology
		{
			Undefined		= 0,
			TriangleList	= 4
		};
		struct DrawCall
		{
			DrawMethod			drawMethod{ DrawMethod::Undefined };
			PrimitiveTopology	primitiveTopology{ PrimitiveTopology::Undefined };
		};
	public:
		Mesh(const DirectX::BoundingBox& aabb, const DrawCall& drawCall, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);
		virtual ~Mesh();
	public:
		virtual void doLoad() override;
	private:
		DirectX::BoundingBox			m_aabb;
		DrawCall						m_drawCall;
		std::shared_ptr<VertexBuffer>	m_vertexBuffer;
		std::shared_ptr<IndexBuffer>	m_indexBuffer;
	};
}