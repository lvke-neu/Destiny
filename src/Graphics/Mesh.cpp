#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Destiny
{
	Mesh::Mesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) :
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
		}
		else
		{
			loadSucceeded__();
		}
	}
}