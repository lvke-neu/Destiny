#include "Mesh.h"
#include "Engine/Utility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderParameters.h"
#include "InputLayout.h"

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

	void Mesh::fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters, std::shared_ptr<Blob> inputSignatureBlob)
	{
		if (!renderParameters || !m_vertexBuffer || !m_vertexBuffer->m_inputLayout ||!m_indexBuffer)
		{
			return;
		}

		renderParameters->vertexBuffer = m_vertexBuffer->m_vertexBuffer;
		renderParameters->stride = m_vertexBuffer->m_stride;
		renderParameters->offset = m_vertexBuffer->m_offset;

		renderParameters->indexBuffer = m_indexBuffer->m_indexBuffer;
		renderParameters->format = (short)m_indexBuffer->m_indexType;

		renderParameters->primitiveTopology = (short)m_drawCall.primitiveTopology;

		renderParameters->inputLayout = m_vertexBuffer->m_inputLayout->getInputLayout(inputSignatureBlob);
		
		renderParameters->drawType = (short)m_drawCall.drawMethod;
		renderParameters->indexCount = m_drawCall.indexCount;
		renderParameters->vertexCount = m_drawCall.vertexCount;
	}
}