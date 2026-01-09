#include "MeshProvider.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "InputLayout.h"
#include "VertexDefine.h"
#include "Engine/Blob.h"
#include "Math/Math.h"

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

	std::shared_ptr<Mesh> MeshProvider::Create_Box_Position()
	{
		auto iter = m_cache.find("Box_Position");
		if (iter != m_cache.end())
		{
			return m_cache["Box_Position"];
		}

		std::shared_ptr<Blob> data = nullptr;
		std::vector<Position3> vertices;

		vertices.push_back({ {-0.5f, -0.5f, 0.5f} });
		vertices.push_back({ {0.5f, -0.5f, 0.5f} });
		vertices.push_back({ {0.5f, 0.5f, 0.5f} });
		vertices.push_back({ {-0.5f, 0.5f, 0.5f} });

		vertices.push_back({ {-0.5f, -0.5f, -0.5f} });
		vertices.push_back({ {0.5f, -0.5f, -0.5f} });
		vertices.push_back({ {0.5f, 0.5f, -0.5f} });
		vertices.push_back({ {-0.5f, 0.5f, -0.5f} });

		data.reset(new Blob(vertices.size() * sizeof(Position3)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(Position3));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(Position3), 0, data);

		std::vector<unsigned short> indices;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);

		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);

		indices.push_back(4); indices.push_back(0); indices.push_back(3);
		indices.push_back(4); indices.push_back(3); indices.push_back(7);

		indices.push_back(1); indices.push_back(5); indices.push_back(6);
		indices.push_back(1); indices.push_back(6); indices.push_back(2);

		indices.push_back(3); indices.push_back(2); indices.push_back(6);
		indices.push_back(3); indices.push_back(6); indices.push_back(7);

		indices.push_back(0); indices.push_back(1); indices.push_back(5);
		indices.push_back(0); indices.push_back(5); indices.push_back(4);


		data.reset(new Blob(indices.size() * sizeof(unsigned short)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

		DirectX::BoundingBox aabb;
		DirectX::BoundingBox::CreateFromPoints(aabb, { -FLT_MAX,-FLT_MAX,-FLT_MAX }, { FLT_MAX,FLT_MAX,FLT_MAX });

		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		m_cache["Box_Position"] = mesh;

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

	std::shared_ptr<Mesh> MeshProvider::Create_Sphere_PositionNormalTexcoord()
	{
		auto iter = m_cache.find("Sphere_PositionNormalTexcoord");
		if (iter != m_cache.end())
		{
			return m_cache["Sphere_PositionNormalTexcoord"];
		}

		const float radius = 0.5f;
		const unsigned int levels = 20;
		const unsigned int slices = 20;

		const unsigned int vertexCount = 2 + (levels - 1) * (slices + 1);


		unsigned int vIndex = 0, iIndex = 0;

		float phi = 0.0f, theta = 0.0f;
		float per_phi = Math::PI / levels;
		float per_theta = 2.0f * Math::PI / slices;
		float x, y, z;

		std::shared_ptr<Blob> data = nullptr;
		std::vector<PositionNormalTexcoord> vertices;
		vertices.resize(vertexCount);

		vertices[vIndex++] = { { 0.0f, radius, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } };
		for (unsigned int i = 1; i < levels; ++i)
		{
			phi = per_phi * i;

			for (unsigned int j = 0; j <= slices; ++j)
			{
				theta = per_theta * j;
				x = radius * sinf(phi) * cosf(theta);
				y = radius * cosf(phi);
				z = radius * sinf(phi) * sinf(theta);

				DirectX::XMFLOAT3 pos = { x, y, z }, normal;
				XMStoreFloat3(&normal, DirectX::XMVector3Normalize(XMLoadFloat3(&pos)));

				vertices[vIndex++] = PositionNormalTexcoord({ pos, normal, {theta / (2.0f * Math::PI), phi / (2.0f * Math::PI)} });
			}
		}
		vertices[vIndex++] = PositionNormalTexcoord({ {0.0f, -radius, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} });


		data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

		std::vector<unsigned short> indices;
		indices.resize(6 * (levels - 1) * slices);
		if (levels > 1)
		{
			for (unsigned int j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = 0;
				indices[iIndex++] = j % (slices + 1) + 1;
				indices[iIndex++] = j;
			}
		}
		for (unsigned int i = 1; i < levels - 1; ++i)
		{
			for (unsigned int j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = (i - 1) * (slices + 1) + j;
				indices[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

				indices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = i * (slices + 1) + j;
				indices[iIndex++] = (i - 1) * (slices + 1) + j;
			}
		}
		if (levels > 1)
		{
			for (unsigned int j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = (levels - 2) * (slices + 1) + j;
				indices[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = (levels - 1) * (slices + 1) + 1;
			}
		}

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

		m_cache["Sphere_PositionNormalTexcoord"] = mesh;

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

		DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		m_cache["Triangle_PositionColor"] = mesh;

		return mesh;
	}

	std::shared_ptr<Mesh> MeshProvider::Create_Position3()
	{
		auto iter = m_cache.find("Position3");
		if (iter != m_cache.end())
		{
			return m_cache["Position3"];
		}

		std::shared_ptr<Blob> data = nullptr;
		std::vector<Position3> vertices;
		vertices.resize(1);

		vertices[0].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

		data.reset(new Blob(vertices.size() * sizeof(Position3)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(Position3));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(Position3), 0, data);


		DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f } };
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::Draw;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::PointList;
		drawCall.indexCount = 0;
		drawCall.vertexCount = 1;

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, nullptr);

		m_cache["Position3"] = mesh;

		return mesh;
	}

	std::shared_ptr<Mesh> MeshProvider::Create_Cone_PositionNormalTexcoord()
{
	auto iter = m_cache.find("Cone_PositionNormalTexcoord");
	if (iter != m_cache.end())
	{
		return m_cache["Cone_PositionNormalTexcoord"];
	}

	const float radius = 1.0f;
	const float height = 3.0f;
	const unsigned int slices = 40;

	const unsigned int vertexCount = slices * 2 + 2;

	std::shared_ptr<Blob> data = nullptr;
	std::vector<PositionNormalTexcoord> vertices;
	vertices.resize(vertexCount);

	// 顶点
	vertices[0].position = DirectX::XMFLOAT3(0.0f, height / 2.0f, 0.0f);
	vertices[0].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[0].texcoord = DirectX::XMFLOAT2(0.5f, 0.0f);

	// 底面圆心
	vertices[slices * 2 + 1].position = DirectX::XMFLOAT3(0.0f, -height / 2.0f, 0.0f);
	vertices[slices * 2 + 1].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[slices * 2 + 1].texcoord = DirectX::XMFLOAT2(0.5f, 0.5f);

	// 侧面的底面圆周顶点
	float theta = 0.0f;
	float per_theta = 2.0f * Math::PI / slices;
	for (unsigned int i = 1; i <= slices; ++i)
	{
		theta = per_theta * i;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		float y = -height / 2.0f;

		vertices[i].position = DirectX::XMFLOAT3(x, y, z);

		// 计算侧面法线
		DirectX::XMFLOAT3 coneDir = { x, height, z };
		DirectX::XMVECTOR coneDirVec = DirectX::XMLoadFloat3(&coneDir);
		coneDirVec = DirectX::XMVector3Normalize(coneDirVec);
		DirectX::XMStoreFloat3(&vertices[i].normal, coneDirVec);

		// 计算侧面纹理坐标
		float u = theta / (2.0f * Math::PI);
		vertices[i].texcoord = DirectX::XMFLOAT2(u, 1.0f);
	}

	// 底面的底面圆周顶点（与侧面共享位置，但使用底面法线和纹理坐标）
	for (unsigned int i = 1; i <= slices; ++i)
	{
		theta = per_theta * i;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		float y = -height / 2.0f;

		unsigned int bottomVertexIndex = slices + i;
		vertices[bottomVertexIndex].position = DirectX::XMFLOAT3(x, y, z);

		// 底面法线
		vertices[bottomVertexIndex].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);

		// 计算底面纹理坐标（从圆心向外辐射）
		float u = 0.5f + 0.5f * cosf(theta);
		float v = 0.5f + 0.5f * sinf(theta);
		vertices[bottomVertexIndex].texcoord = DirectX::XMFLOAT2(u, v);
	}

	data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
	data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

	// 创建索引数据
	std::vector<unsigned short> indices;
	// 侧面三角形 - 修正为DX11顺时针绕序
	for (unsigned int i = 1; i <= slices; ++i)
	{
		indices.push_back(0);
		indices.push_back(i % slices + 1);
		indices.push_back(i);
	}
	// 底面三角形 - 修正为DX11顺时针绕序
	for (unsigned int i = 1; i <= slices; ++i)
	{
		indices.push_back(slices * 2 + 1);
		indices.push_back(slices + i);
		indices.push_back(slices + (i % slices + 1));
	}

	data.reset(new Blob(indices.size() * sizeof(unsigned short)));
	data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

	// 创建包围盒
	DirectX::BoundingBox aabb;
	DirectX::BoundingBox::CreateFromPoints(aabb, { -radius, -height / 2.0f, -radius }, { radius, height / 2.0f, radius });

	// 创建绘制调用
	Mesh::DrawCall drawCall;
	drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
	drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
	drawCall.indexCount = (unsigned int)indices.size();

	// 创建网格对象
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

	// 添加到缓存
	m_cache["Cone_PositionNormalTexcoord"] = mesh;

	return mesh;
}

std::shared_ptr<Mesh> MeshProvider::Create_Cylinder_PositionNormalTexcoord()
{
	auto iter = m_cache.find("Cylinder_PositionNormalTexcoord");
	if (iter != m_cache.end())
	{
		return m_cache["Cylinder_PositionNormalTexcoord"];
	}

	const float radius = 1.0f;
	const float height = 3.0f;
	const unsigned int slices = 40;

	// 顶点数量：上下底面中心各1个，上下底面圆周各slices个，侧面slices*2个
	const unsigned int vertexCount = 2 + slices * 4;

	std::shared_ptr<Blob> data = nullptr;
	std::vector<PositionNormalTexcoord> vertices;
	vertices.resize(vertexCount);

	float theta = 0.0f;
	float per_theta = 2.0f * Math::PI / slices;

	// 上底面中心
	vertices[0].position = DirectX::XMFLOAT3(0.0f, height / 2.0f, 0.0f);
	vertices[0].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[0].texcoord = DirectX::XMFLOAT2(0.5f, 0.5f);

	// 下底面中心
	vertices[1].position = DirectX::XMFLOAT3(0.0f, -height / 2.0f, 0.0f);
	vertices[1].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[1].texcoord = DirectX::XMFLOAT2(0.5f, 0.5f);

	// 上底面圆周顶点
	for (unsigned int i = 0; i < slices; ++i)
	{
		theta = per_theta * i;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		float y = height / 2.0f;

		unsigned int vertexIndex = 2 + i;
		vertices[vertexIndex].position = DirectX::XMFLOAT3(x, y, z);
		vertices[vertexIndex].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

		// 上底面纹理坐标 - 从中心向外辐射
		float u = 0.5f + 0.5f * cosf(theta);
		float v = 0.5f + 0.5f * sinf(theta);
		vertices[vertexIndex].texcoord = DirectX::XMFLOAT2(u, v);
	}

	// 下底面圆周顶点
	for (unsigned int i = 0; i < slices; ++i)
	{
		theta = per_theta * i;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		float y = -height / 2.0f;

		unsigned int vertexIndex = 2 + slices + i;
		vertices[vertexIndex].position = DirectX::XMFLOAT3(x, y, z);
		vertices[vertexIndex].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);

		// 下底面纹理坐标 - 从中心向外辐射
		float u = 0.5f + 0.5f * cosf(theta);
		float v = 0.5f + 0.5f * sinf(theta);
		vertices[vertexIndex].texcoord = DirectX::XMFLOAT2(u, v);
	}

	// 侧面上半部分顶点
	for (unsigned int i = 0; i < slices; ++i)
	{
		theta = per_theta * i;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		float y = height / 2.0f;

		unsigned int vertexIndex = 2 + slices * 2 + i;
		vertices[vertexIndex].position = DirectX::XMFLOAT3(x, y, z);

		// 侧面法线 - 径向向外
		DirectX::XMFLOAT3 normal = { x, 0.0f, z };
		DirectX::XMVECTOR normalVec = DirectX::XMLoadFloat3(&normal);
		normalVec = DirectX::XMVector3Normalize(normalVec);
		DirectX::XMStoreFloat3(&vertices[vertexIndex].normal, normalVec);

		// 侧面纹理坐标
		float u = (float)i / (float)slices;
		float v = 1.0f;
		vertices[vertexIndex].texcoord = DirectX::XMFLOAT2(u, v);
	}

	// 侧面下半部分顶点
	for (unsigned int i = 0; i < slices; ++i)
	{
		theta = per_theta * i;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		float y = -height / 2.0f;

		unsigned int vertexIndex = 2 + slices * 3 + i;
		vertices[vertexIndex].position = DirectX::XMFLOAT3(x, y, z);

		// 侧面法线 - 径向向外
		DirectX::XMFLOAT3 normal = { x, 0.0f, z };
		DirectX::XMVECTOR normalVec = DirectX::XMLoadFloat3(&normal);
		normalVec = DirectX::XMVector3Normalize(normalVec);
		DirectX::XMStoreFloat3(&vertices[vertexIndex].normal, normalVec);

		// 侧面纹理坐标
		float u = (float)i / (float)slices;
		float v = 0.0f;
		vertices[vertexIndex].texcoord = DirectX::XMFLOAT2(u, v);
	}

	data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
	data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

	// 创建索引数据
	std::vector<unsigned short> indices;

	// 上底面三角形 - 修正为DX11顺时针绕序（从上往下看）
	for (unsigned int i = 0; i < slices; ++i)
	{
		unsigned int next = (i + 1) % slices;
		indices.push_back(0);  // 中心
		indices.push_back(2 + i);  // 当前顶点
		indices.push_back(2 + next);  // 下一个顶点
	}

	// 下底面三角形 - 修正为DX11顺时针绕序（从下往上看）
	for (unsigned int i = 0; i < slices; ++i)
	{
		unsigned int next = (i + 1) % slices;
		indices.push_back(1);  // 中心
		indices.push_back(2 + slices + next);  // 下一个顶点
		indices.push_back(2 + slices + i);  // 当前顶点
	}

	// 侧面矩形 - 修正为DX11顺时针绕序（从外部看）
	for (unsigned int i = 0; i < slices; ++i)
	{
		unsigned int next = (i + 1) % slices;
		unsigned int top1 = 2 + slices * 2 + i;
		unsigned int top2 = 2 + slices * 2 + next;
		unsigned int bottom1 = 2 + slices * 3 + i;
		unsigned int bottom2 = 2 + slices * 3 + next;

		// 第一个三角形 - 顺时针绕序
		indices.push_back(top1);
		indices.push_back(bottom1);
		indices.push_back(top2);

		// 第二个三角形 - 顺时针绕序
		indices.push_back(top2);
		indices.push_back(bottom1);
		indices.push_back(bottom2);
	}

	data.reset(new Blob(indices.size() * sizeof(unsigned short)));
	data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

	// 创建包围盒
	DirectX::BoundingBox aabb;
	DirectX::BoundingBox::CreateFromPoints(aabb, { -radius, -height / 2.0f, -radius }, { radius, height / 2.0f, radius });

	// 创建绘制调用
	Mesh::DrawCall drawCall;
	drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
	drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
	drawCall.indexCount = (unsigned int)indices.size();

	// 创建网格对象
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

	// 添加到缓存
	m_cache["Cylinder_PositionNormalTexcoord"] = mesh;

	return mesh;
}

std::shared_ptr<Mesh> MeshProvider::Create_Torus_PositionNormalTexcoord()
{
	auto iter = m_cache.find("Torus_PositionNormalTexcoord");
	if (iter != m_cache.end())
	{
		return m_cache["Torus_PositionNormalTexcoord"];
	}

	const float majorRadius = 5.0f;  // 外半径
	const float minorRadius = 2.0f;  // 内半径
	const unsigned int majorSegments = 32; // 外圆分段数
	const unsigned int minorSegments = 16; // 内圆分段数

	const unsigned int vertexCount = majorSegments * minorSegments;
	const unsigned int indexCount = majorSegments * minorSegments * 6;

	std::shared_ptr<Blob> data = nullptr;
	std::vector<PositionNormalTexcoord> vertices;
	vertices.resize(vertexCount);

	std::vector<unsigned short> indices;
	indices.resize(indexCount);

	unsigned int vertexIndex = 0;
	unsigned int indexIndex = 0;

	for (unsigned int i = 0; i < majorSegments; ++i)
	{
		float majorTheta = (float)i / (float)majorSegments * 2.0f * Math::PI;
		DirectX::XMFLOAT3 majorCirclePos = {
			majorRadius * cosf(majorTheta),
			0.0f,
			majorRadius * sinf(majorTheta)
		};

		for (unsigned int j = 0; j < minorSegments; ++j)
		{
			float minorTheta = (float)j / (float)minorSegments * 2.0f * Math::PI;

			// 计算顶点位置
			float x = (majorRadius + minorRadius * cosf(minorTheta)) * cosf(majorTheta);
			float y = minorRadius * sinf(minorTheta);
			float z = (majorRadius + minorRadius * cosf(minorTheta)) * sinf(majorTheta);

			vertices[vertexIndex].position = DirectX::XMFLOAT3(x, y, z);

			// 计算法线
			DirectX::XMFLOAT3 normal = {
				cosf(majorTheta) * cosf(minorTheta),
				sinf(minorTheta),
				sinf(majorTheta) * cosf(minorTheta)
			};
			vertices[vertexIndex].normal = normal;

			// 计算纹理坐标
			vertices[vertexIndex].texcoord = DirectX::XMFLOAT2(
				(float)i / (float)majorSegments,
				(float)j / (float)minorSegments
			);

			// 生成索引
			unsigned int nextMajor = (i + 1) % majorSegments;
			unsigned int nextMinor = (j + 1) % minorSegments;

			unsigned int currentVertex = vertexIndex;
			unsigned int nextMajorVertex = nextMajor * minorSegments + j;
			unsigned int nextVertex = vertexIndex + 1;
			unsigned int nextMajorNextVertex = nextMajor * minorSegments + nextMinor;

			// 处理最后一个小段的特殊情况
			if (j == minorSegments - 1)
			{
				nextVertex = i * minorSegments;
				nextMajorNextVertex = nextMajor * minorSegments;
			}

			// 第一个三角形 - 顺时针绕序 (D3D11默认前向)
			indices[indexIndex++] = (unsigned short)currentVertex;
			indices[indexIndex++] = (unsigned short)nextMajorVertex;
			indices[indexIndex++] = (unsigned short)nextVertex;

			// 第二个三角形 - 顺时针绕序 (D3D11默认前向)
			indices[indexIndex++] = (unsigned short)nextVertex;
			indices[indexIndex++] = (unsigned short)nextMajorVertex;
			indices[indexIndex++] = (unsigned short)nextMajorNextVertex;

			++vertexIndex;
		}
	}

	data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
	data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

	data.reset(new Blob(indices.size() * sizeof(unsigned short)));
	data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

	// 创建包围盒
	DirectX::BoundingBox aabb;
	float extent = majorRadius + minorRadius;
	DirectX::BoundingBox::CreateFromPoints(aabb, { -extent, -minorRadius, -extent }, { extent, minorRadius, extent });

	// 创建绘制调用
	Mesh::DrawCall drawCall;
	drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
	drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
	drawCall.indexCount = (unsigned int)indices.size();

	// 创建网格对象
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

	// 添加到缓存
	m_cache["Torus_PositionNormalTexcoord"] = mesh;

	return mesh;
}

std::shared_ptr<Mesh> MeshProvider::Create_Capsule_PositionNormalTexcoord()
{
	auto iter = m_cache.find("Capsule_PositionNormalTexcoord");
	if (iter != m_cache.end())
	{
		return m_cache["Capsule_PositionNormalTexcoord"];
	}

	const float radius = 0.25f;  // 胶囊体半径
	const float height = 0.5f;    // 圆柱体部分高度（不包含两个半球）
	const unsigned int slices = 20;   // 径向分段数
	const unsigned int stacks = 12;   // 轴向分段数

	// 确保stacks至少为4，以保证有足够的层数
	const unsigned int safeStacks = std::max(4U, stacks);

	// 计算各部分的分段数 - 确保至少有2个圆柱体分段
	const unsigned int hemisphereStacks = std::max(1U, (safeStacks - 2) / 2);
	const unsigned int cylinderStacks = std::max(2U, safeStacks - hemisphereStacks * 2);

	// 计算总顶点数和索引数
	// 上半球: hemisphereStacks + 1 层
	// 圆柱体: cylinderStacks - 1 层（排除与上半球重叠的第一层）
	// 下半球: hemisphereStacks + 1 层
	const unsigned int totalLayers = (hemisphereStacks + 1) + (cylinderStacks - 1) + (hemisphereStacks + 1);
	const unsigned int vertexCount = totalLayers * slices;
	const unsigned int indexCount = (totalLayers - 1) * slices * 6;

	std::shared_ptr<Blob> data = nullptr;
	std::vector<PositionNormalTexcoord> vertices;
	vertices.resize(vertexCount);

	std::vector<unsigned short> indices;
	indices.reserve(indexCount);

	unsigned int vertexIndex = 0;

	// 生成顶点数据
	float theta = 0.0f;
	float per_theta = 2.0f * Math::PI / slices;
	float cylinderPerHeight = height / cylinderStacks;

	// 1. 上半球顶点 - 从顶点(y=height/2+radius)到圆柱体顶部(y=height/2)
	// 注意：必须从上到下生成，以匹配索引生成的顺序
	for (unsigned int i = 0; i <= hemisphereStacks; ++i)
	{
		// phi角从π/2（顶部）到0（与圆柱体连接的赤道）
		float phi = Math::PI / 2.0f - (Math::PI / 2.0f / hemisphereStacks) * i;
		float y = height / 2.0f + radius * sinf(phi);
		float r = radius * cosf(phi);

		for (unsigned int j = 0; j < slices; ++j)
		{
			theta = per_theta * j;
			float x = r * cosf(theta);
			float z = r * sinf(theta);

			vertices[vertexIndex].position = DirectX::XMFLOAT3(x, y, z);
			vertices[vertexIndex].normal = DirectX::XMFLOAT3(x / radius, sinf(phi), z / radius);
			vertices[vertexIndex].texcoord = DirectX::XMFLOAT2((float)j / slices, (float)i / (hemisphereStacks + cylinderStacks + hemisphereStacks));

			++vertexIndex;
		}
	}

	// 2. 中间圆柱体顶点 - 从y=height/2到底y=-height/2
	// 注意：从i=1开始，因为i=0的层（y=height/2）已经由上半球最后一层生成
	for (unsigned int i = 1; i < cylinderStacks; ++i)
	{
		float y = height / 2.0f - cylinderPerHeight * i;

		for (unsigned int j = 0; j < slices; ++j)
		{
			theta = per_theta * j;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);

			vertices[vertexIndex].position = DirectX::XMFLOAT3(x, y, z);
			vertices[vertexIndex].normal = DirectX::XMFLOAT3(x / radius, 0.0f, z / radius);
			vertices[vertexIndex].texcoord = DirectX::XMFLOAT2((float)j / slices, (float)(hemisphereStacks + i) / (hemisphereStacks + cylinderStacks + hemisphereStacks));

			++vertexIndex;
		}
	}

	// 3. 下半球顶点 - 从圆柱体底部(y=-height/2)到半球顶点(y=-height/2-radius)
	for (unsigned int i = 0; i <= hemisphereStacks; ++i)
	{
		// phi角从0（与圆柱体连接的赤道）到π/2（底部）
		float phi = (Math::PI / 2.0f / hemisphereStacks) * i;
		float y = -height / 2.0f - radius * sinf(phi);
		float r = radius * cosf(phi);

		for (unsigned int j = 0; j < slices; ++j)
		{
			theta = per_theta * j;
			float x = r * cosf(theta);
			float z = r * sinf(theta);

			vertices[vertexIndex].position = DirectX::XMFLOAT3(x, y, z);
			vertices[vertexIndex].normal = DirectX::XMFLOAT3(x / radius, -sinf(phi), z / radius);
			vertices[vertexIndex].texcoord = DirectX::XMFLOAT2((float)j / slices, (float)(hemisphereStacks + cylinderStacks + i) / (hemisphereStacks + cylinderStacks + hemisphereStacks));

			++vertexIndex;
		}
	}

	// 生成索引 - 使用totalLayers-1而不是stacks
	for (unsigned int i = 0; i < totalLayers - 1; ++i)
	{
		for (unsigned int j = 0; j < slices; ++j)
		{
			unsigned int current = i * slices + j;
			unsigned int next = (i + 1) * slices + j;
			unsigned int currentNext = i * slices + ((j + 1) % slices);
			unsigned int nextNext = (i + 1) * slices + ((j + 1) % slices);

			// 第一个三角形 - 顺时针绕序 (D3D11默认前向)
			indices.push_back((unsigned short)current);
			indices.push_back((unsigned short)currentNext);
			indices.push_back((unsigned short)next);

			// 第二个三角形 - 顺时针绕序 (D3D11默认前向)
			indices.push_back((unsigned short)next);
			indices.push_back((unsigned short)currentNext);
			indices.push_back((unsigned short)nextNext);
		}
	}

	data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
	data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

	data.reset(new Blob(indices.size() * sizeof(unsigned short)));
	data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

	// 创建包围盒
	DirectX::BoundingBox aabb;
	float yExtent = height / 2.0f + radius;
	float xzExtent = radius;
	DirectX::BoundingBox::CreateFromPoints(aabb, { -xzExtent, -yExtent, -xzExtent }, { xzExtent, yExtent, xzExtent });

	// 创建绘制调用
	Mesh::DrawCall drawCall;
	drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
	drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
	drawCall.indexCount = (unsigned int)indices.size();

	// 创建网格对象
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

	// 添加到缓存
	m_cache["Capsule_PositionNormalTexcoord"] = mesh;

	return mesh;
}

std::shared_ptr<Mesh> MeshProvider::Create_FullScreenTriangle()
{
	auto iter = m_cache.find("FullScreenTriangle");
	if (iter != m_cache.end())
	{
		return m_cache["FullScreenTriangle"];
	}

	DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };
	Mesh::DrawCall drawCall;
	drawCall.drawMethod = Mesh::DrawMethod::Draw;
	drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
	drawCall.indexCount = 0;
	drawCall.vertexCount = 3;

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, nullptr, nullptr);

	m_cache["FullScreenTriangle"] = mesh;

	return mesh;
}

	std::shared_ptr<Mesh> MeshProvider::Create_Water(float width, float depth, unsigned int m, unsigned int n)
	{
		std::string key = "Water_" + std::to_string(width) + "_" + std::to_string(depth) + "_" + std::to_string(m) + "_" + std::to_string(n);

		auto iter = m_cache.find(key);
		if (iter != m_cache.end())
		{
			return m_cache[key];
		}

		unsigned int vertexCount = m * n;
		unsigned int faceCount = (m - 1) * (n - 1) * 2;

		std::shared_ptr<Blob> data = nullptr;

		std::vector<PositionTexcoord> vertices;
		vertices.resize(vertexCount);

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (m - 1);
		float dz = depth / (n - 1);

		float du = 1.0f / (m - 1);
		float dv = 1.0f / (n - 1);

		for (unsigned int i = 0; i < n; ++i)
		{
			float z = halfDepth - i * dz;
			for (unsigned int j = 0; j < m; ++j)
			{
				float x = -halfWidth + j * dx;

				vertices[i * m + j].position = DirectX::XMFLOAT3(x, 0.0f, z);
				vertices[i * m + j].texcoord = DirectX::XMFLOAT2(j * du, i * dv);
			}
		}

		data.reset(new Blob(vertices.size() * sizeof(PositionTexcoord)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionTexcoord));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionTexcoord(), (unsigned int)sizeof(PositionTexcoord), 0, data);

		std::vector<unsigned int> indices;
		indices.resize(faceCount * 3);
		unsigned int k = 0;
		for (unsigned int i = 0; i < n - 1; ++i)
		{
			for (unsigned int j = 0; j < m - 1; ++j)
			{
				indices[k] = i * m + j;
				indices[k + 1] = i * m + j + 1;
				indices[k + 2] = (i + 1) * m + j;

				indices[k + 3] = (i + 1) * m + j;
				indices[k + 4] = i * m + j + 1;
				indices[k + 5] = (i + 1) * m + j + 1;

				k += 6;
			}
		}

		data.reset(new Blob(indices.size() * sizeof(unsigned int)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned int));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index32, data);

		DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();


		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		m_cache[key] = mesh;

		return mesh;
	}
}