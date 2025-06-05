#pragma once
#include "Engine/Asset.h"
#include <DirectXCollision.h>

namespace Destiny
{
	class VertexBuffer;
	class InstanceBuffer;
	class IndexBuffer;
	class DrawParameters;
	class Blob;
	class Mesh : public Asset
	{
	public:
		enum class DrawMethod
		{
			Undefined,
			Draw,
			DrawIndexed,
			DrawIndexedInstanced
		};
		enum class PrimitiveTopology
		{
			Undefined		= 0,
			PointList		= 1,
			LineList		= 2,
			TriangleList	= 4,
			PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST = 35
		};
		struct DrawCall
		{
			DrawMethod					drawMethod{ DrawMethod::Undefined };
			PrimitiveTopology			primitiveTopology{ PrimitiveTopology::Undefined };
			unsigned int indexCount		= 0;
			unsigned int vertexCount	= 0;
		};
	public:
		Mesh(const DrawCall& drawCall, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<InstanceBuffer> instanceBuffer = nullptr);
		Mesh(const DirectX::BoundingBox& aabb, const DrawCall& drawCall, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<InstanceBuffer> instanceBuffer = nullptr);
		virtual ~Mesh();
	public:
		virtual void doLoad() override;
	public:
		void							updateInstanceBufferData(std::shared_ptr<Blob> data);
		void							fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters, std::shared_ptr<Blob> inputSignatureBlob);
		DirectX::BoundingBox			getBoundingBox();
		void							setBoundingBox(const DirectX::BoundingBox& aabb);
		DrawCall						getDrawCall();
		void							modifyVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
		void							modifyDrawCall(const DrawCall& drawCall);
	private:
		DirectX::BoundingBox			m_aabb;
		DrawCall						m_drawCall;
		std::shared_ptr<VertexBuffer>	m_vertexBuffer;
		std::shared_ptr<InstanceBuffer>	m_instanceBuffer;
		std::shared_ptr<IndexBuffer>	m_indexBuffer;
	};

	inline DirectX::BoundingBox Mesh::getBoundingBox()
	{
		return m_aabb;
	}

	inline void Mesh::setBoundingBox(const DirectX::BoundingBox& aabb)
	{
		m_aabb = aabb;
	}

	inline Mesh::DrawCall Mesh::getDrawCall()
	{
		return m_drawCall;
	}
}