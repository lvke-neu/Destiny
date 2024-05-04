#pragma once
#include "Engine/Asset.h"
#include <DirectXCollision.h>

namespace Destiny
{
	class VertexBuffer;
	class IndexBuffer;
	class RenderParameters;
	class Blob;
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
			DrawMethod					drawMethod{ DrawMethod::Undefined };
			PrimitiveTopology			primitiveTopology{ PrimitiveTopology::Undefined };
			unsigned int indexCount		= 0;
			unsigned int vertexCount	= 0;
		};
	public:
		Mesh(const DirectX::BoundingBox& aabb, const DrawCall& drawCall, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);
		virtual ~Mesh();
	public:
		virtual void doLoad() override;
	public:
		void fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters, std::shared_ptr<Blob> inputSignatureBlob);
	private:
		DirectX::BoundingBox			m_aabb;
		DrawCall						m_drawCall;
		std::shared_ptr<VertexBuffer>	m_vertexBuffer;
		std::shared_ptr<IndexBuffer>	m_indexBuffer;
	};
}