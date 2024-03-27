#include "Mesh.h"
#include "Engine/Utility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Destiny
{
	Mesh::Mesh(const DirectX::BoundingBox& aabb, const DrawCall& drawCall, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) :
		m_aabb(aabb),
		m_drawCall(drawCall),
		m_vertexBuffer(vertexBuffer),
		m_indexBuffer(indexBuffer)
	{

	}

	Mesh::~Mesh()
	{

	}

	void Mesh::doLoad()
	{
		if (!m_vertexBuffer || !m_indexBuffer)
		{
			loadFailed__();
			LOG_ERROR("CreateMesh failed");
			return;
		}

		if (m_vertexBuffer->isLoadingPending())
		{
			m_vertexBuffer->load(0);
		}

		if (m_indexBuffer->isLoadingPending())
		{
			m_indexBuffer->load(0);
		}

		if (!m_vertexBuffer->isLoadingSucceed() || !m_indexBuffer->isLoadingSucceed())
		{
			loadFailed__();
			LOG_ERROR("CreateMesh failed");
		}
		else
		{
			loadSucceeded__();
		}
	}
}