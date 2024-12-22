#include "MeshProvider.h"
#include "Engine/Blob.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "InputLayout.h"
#include "VertexDefine.h"

namespace Destiny
{
	std::unordered_map<std::string, std::shared_ptr<Mesh>> MeshProvider::m_cache;
	std::shared_ptr<Mesh> MeshProvider::Create_Box_PositionNormalTexcoord()
	{
		auto iter = m_cache.find("Box_PositionNormalTexcoord");
		if (iter != m_cache.end())
		{
			return m_cache["Box_PositionNormalTexcoord"];
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
		for (unsigned int i = 0; i < 4; ++i)
		{
			vertices[i].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[i + 4].normal = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertices[i + 8].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[i + 12].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[i + 16].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertices[i + 20].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		}
		for (unsigned int i = 0; i < 6; ++i)
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

		DirectX::BoundingBox aabb;
		DirectX::BoundingBox::CreateFromPoints(aabb, { -1.0f,-1.0f,-1.0f }, { 1.0f,1.0f,1.0f });

		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		m_cache["Box_PositionNormalTexcoord"] = mesh;

		return mesh;
	}

	std::shared_ptr<Mesh> MeshProvider::Create_Plane_PositionNormalTexcoord()
	{
		auto iter = m_cache.find("Plane_PositionNormalTexcoord");
		if (iter != m_cache.end())
		{
			return m_cache["Plane_PositionNormalTexcoord"];
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

		DirectX::BoundingBox aabb;
		DirectX::BoundingBox::CreateFromPoints(aabb, { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f });
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		m_cache["Plane_PositionNormalTexcoord"] = mesh;

		return mesh;
	}

	std::shared_ptr<Mesh> MeshProvider::Create_Triangle_PositionColor()
	{
		auto iter = m_cache.find("Triangle_PositionColor");
		if (iter != m_cache.end())
		{
			return m_cache["Triangle_PositionColor"];
		}

		std::shared_ptr<Blob> data = nullptr;
		std::vector<PositionColor> vertices;
		vertices.resize(3);

		vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f);
		vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[1].position = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f);
		vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		vertices[2].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
		vertices[2].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);


		data.reset(new Blob(vertices.size() * sizeof(PositionColor)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionColor));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionColor(), (unsigned int)sizeof(PositionColor), 0, data);

		std::vector<unsigned short> indices =
		{
			0, 1, 2
		};
		data.reset(new Blob(indices.size() * sizeof(unsigned short)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

		DirectX::BoundingBox aabb { { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		m_cache["Triangle_PositionColor"] = mesh;

		return mesh;
	}

	std::shared_ptr<Mesh> MeshProvider::Create_Position()
	{
		auto iter = m_cache.find("Position");
		if (iter != m_cache.end())
		{
			return m_cache["Position"];
		}

		std::shared_ptr<Blob> data = nullptr;
		std::vector<Position> vertices;
		vertices.resize(1);

		vertices[0].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

		data.reset(new Blob(vertices.size() * sizeof(PositionColor)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionColor));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position(), (unsigned int)sizeof(PositionColor), 0, data);

	
		DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f } };
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::Draw;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::PointList;
		drawCall.indexCount = 0;
		drawCall.vertexCount = 1;

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, nullptr);

		m_cache["Triangle_PositionColor"] = mesh;

		return mesh;
	}
}