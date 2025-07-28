#include "Mesh.h"
#include "VertexBuffer.h"
#include "InstanceBuffer.h"
#include "IndexBuffer.h"
#include "DrawParameters.h"
#include "InputLayout.h"
#include "IndirectBuffer.h"
#include "Engine/Utility.h"

namespace Destiny
{
	Mesh::Mesh(const DrawCall& drawCall, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<InstanceBuffer> instanceBuffer) :
		m_drawCall(drawCall),
		m_vertexBuffer(vertexBuffer),
		m_indexBuffer(indexBuffer),
		m_instanceBuffer(instanceBuffer),
		m_indirectBuffer(nullptr)
	{
		m_aabb = { { 0.0f, 0.0f, 0.0f },{ -1.0f, -1.0f, -1.0 } };
	}

	Mesh::Mesh(const DirectX::BoundingBox& aabb, const DrawCall& drawCall, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<InstanceBuffer> instanceBuffer) :
		m_aabb(aabb),
		m_drawCall(drawCall),
		m_vertexBuffer(vertexBuffer),
		m_indexBuffer(indexBuffer),
		m_instanceBuffer(instanceBuffer),
		m_indirectBuffer(nullptr)
	{

	}

	Mesh::~Mesh()
	{

	}

	void Mesh::doLoad()
	{
		if (isLoadingSucceed() || isLoadingStart())
		{
			return;
		}

		loadStart__();

		if (m_vertexBuffer && m_vertexBuffer->isLoadingPending())
		{
			m_vertexBuffer->load(0);
		}

		if (m_indexBuffer && m_indexBuffer->isLoadingPending())
		{
			m_indexBuffer->load(0);
		}

		//if ((m_vertexBuffer && !m_vertexBuffer->isLoadingSucceed()) ||
		//	(m_indexBuffer && !m_indexBuffer->isLoadingSucceed()) )
		//{
		//	loadFailed__();
		//	LOG_ERROR("CreateMesh failed");
		//}
		//else
		//{
		//	loadSucceeded__();
		//}
	}

	void Mesh::updateInstanceBufferData(std::shared_ptr<Blob> data)
	{
		if (m_instanceBuffer)
		{
			m_instanceBuffer->updateInstanceData(data);
		}
	}

	void Mesh::fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters, std::shared_ptr<Blob> inputSignatureBlob)
	{
		if (!drawParameters)
		{
			return;
		}

		if (m_vertexBuffer)
		{
			drawParameters->vertexBuffer = m_vertexBuffer->m_vertexBuffer;
			drawParameters->vertexBuffer_stride = m_vertexBuffer->m_stride;
			drawParameters->vertexBuffer_offset = m_vertexBuffer->m_offset;
			
			if (m_vertexBuffer->m_inputLayout)
			{
				drawParameters->inputLayout = m_vertexBuffer->m_inputLayout->getInputLayout(inputSignatureBlob);
			}
		}

		if (m_indexBuffer)
		{
			drawParameters->indexBuffer = m_indexBuffer->m_indexBuffer;
			drawParameters->format = (short)m_indexBuffer->m_indexType;
		}

		drawParameters->primitiveTopology = (short)m_drawCall.primitiveTopology;		
		drawParameters->drawType = (short)m_drawCall.drawMethod;
		drawParameters->indexCount = m_drawCall.indexCount;
		drawParameters->vertexCount = m_drawCall.vertexCount;

		if (m_instanceBuffer)
		{
			drawParameters->instanceBuffer = m_instanceBuffer->m_instanceBuffer;
			drawParameters->instanceBuffer_stride = m_instanceBuffer->m_stride;
			drawParameters->instanceBuffer_offset = m_instanceBuffer->m_offset;
			drawParameters->instanceCount = m_instanceBuffer->m_instanceCount;
		}

		if (m_indirectBuffer)
		{
			std::vector<std::pair<short, unsigned int>>		tmp_drawIndirectMethod_indirectBufferOffsets;
			for (const auto& drawIndirectMethod_indirectBufferOffset : m_drawIndirectMethod_indirectBufferOffsets)
			{
				tmp_drawIndirectMethod_indirectBufferOffsets.push_back({(short)drawIndirectMethod_indirectBufferOffset .first, drawIndirectMethod_indirectBufferOffset .second});
			}

			drawParameters->indirectBuffer = m_indirectBuffer->m_indirectBuffer;
			drawParameters->drawIndirectMethod_indirectBufferOffsets = tmp_drawIndirectMethod_indirectBufferOffsets;
		}
	}

	void Mesh::modifyVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		if (!vertexBuffer)
		{
			return;
		}

		m_vertexBuffer = vertexBuffer;
		if (m_vertexBuffer->isLoadingPending())
		{
			m_vertexBuffer->load(0);
		}
	}

	void Mesh::modifyDrawCall(const DrawCall& drawCall)
	{
		m_drawCall = drawCall;
	}

	void Mesh::modifyBoundingBox(const DirectX::BoundingBox& box)
	{
		m_aabb = box;
	}

	void Mesh::setIndirectMode(std::shared_ptr<IndirectBuffer> indirectBuffer, const std::vector<std::pair<DrawIndirectMethod, unsigned int>>& drawIndirectMethod_indirectBufferOffsets)
	{
		m_indirectBuffer = indirectBuffer;
		m_drawIndirectMethod_indirectBufferOffsets = drawIndirectMethod_indirectBufferOffsets;
	}
}