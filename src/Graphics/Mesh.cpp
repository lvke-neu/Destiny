#include "Mesh.h"
#include "Engine/Utility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DrawParameters.h"
#include "InputLayout.h"
#include "Engine/Blob.h"

namespace Destiny
{
	std::unordered_map<std::string, std::shared_ptr<Mesh>> Mesh::m_cache;

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
		if (isLoadingSucceed())
		{
			return;
		}

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

	void Mesh::fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters, std::shared_ptr<Blob> inputSignatureBlob)
	{
		if (!drawParameters || !m_vertexBuffer || !m_vertexBuffer->m_inputLayout ||!m_indexBuffer)
		{
			return;
		}

		drawParameters->vertexBuffer = m_vertexBuffer->m_vertexBuffer;
		drawParameters->stride = m_vertexBuffer->m_stride;
		drawParameters->offset = m_vertexBuffer->m_offset;

		drawParameters->indexBuffer = m_indexBuffer->m_indexBuffer;
		drawParameters->format = (short)m_indexBuffer->m_indexType;

		drawParameters->primitiveTopology = (short)m_drawCall.primitiveTopology;

		drawParameters->inputLayout = m_vertexBuffer->m_inputLayout->getInputLayout(inputSignatureBlob);
		
		drawParameters->drawType = (short)m_drawCall.drawMethod;
		drawParameters->indexCount = m_drawCall.indexCount;
		drawParameters->vertexCount = m_drawCall.vertexCount;
	}

	struct PositionNormalTexcoord
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};

	std::shared_ptr<Mesh> Mesh::Create_Box_PositionNormalTexcoord()
	{
		auto iter = m_cache.find("Box");
		if (iter != m_cache.end())
		{
			return m_cache["Box"];
		}

		std::shared_ptr<Blob> data = nullptr;
		std::vector<PositionNormalTexcoord> vertices;
		vertices.resize(24);

		vertices[0].position = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f);
		vertices[1].position = DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f);
		vertices[2].position = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		vertices[3].position = DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f);
		vertices[4].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f);
		vertices[5].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f);
		vertices[6].position = DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f);
		vertices[7].position = DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f);
		vertices[8].position = DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f);
		vertices[9].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f);
		vertices[10].position = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		vertices[11].position = DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f);
		vertices[12].position = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f);
		vertices[13].position = DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f);
		vertices[14].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f);
		vertices[15].position = DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f);
		vertices[16].position = DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f);
		vertices[17].position = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		vertices[18].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f);
		vertices[19].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f);
		vertices[20].position = DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f);
		vertices[21].position = DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f);
		vertices[22].position = DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f);
		vertices[23].position = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f);
		for (UINT i = 0; i < 4; ++i)
		{
			vertices[i].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[i + 4].normal = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertices[i + 8].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[i + 12].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[i + 16].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertices[i + 20].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		}
		for (UINT i = 0; i < 6; ++i)
		{
			vertices[i * 4].texcoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[i * 4 + 1].texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			vertices[i * 4 + 2].texcoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[i * 4 + 3].texcoord = DirectX::XMFLOAT2(1.0f, 1.0f);

		}
		data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

		std::vector<unsigned short> indices = 
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};
		data.reset(new Blob(indices.size() * sizeof(unsigned short)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

		DirectX::BoundingBox aabb({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f });
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		m_cache["Box"] = mesh;

		return mesh;
	}
	std::shared_ptr<Mesh> Mesh::Create_Plane_PositionNormalTexcoord()
	{
		auto iter = m_cache.find("Plane");
		if (iter != m_cache.end())
		{
			return m_cache["Plane"];
		}

		std::shared_ptr<Blob> data = nullptr;
		std::vector<PositionNormalTexcoord> vertices;
		vertices.resize(4);

		vertices[0].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
		vertices[0].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[0].texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);

		vertices[1].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
		vertices[1].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[1].texcoord = DirectX::XMFLOAT2(1.0f, 0.0f);

		vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertices[2].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[2].texcoord = DirectX::XMFLOAT2(1.0f, 1.0f);

		vertices[3].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertices[3].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[3].texcoord = DirectX::XMFLOAT2(0.0f, 1.0f);

		data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

		std::vector<unsigned short> indices =
		{
			0, 1, 3, 1, 2, 3
		};
		data.reset(new Blob(indices.size() * sizeof(unsigned short)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

		DirectX::BoundingBox aabb({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f });
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		m_cache["Plane"] = mesh;

		return mesh;
	}
}