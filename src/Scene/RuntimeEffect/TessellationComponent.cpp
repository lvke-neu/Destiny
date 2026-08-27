#include "TessellationComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/VertexDefine.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Texture.h"

namespace Destiny
{
	TessellationComponent::TessellationComponent() :
		m_maxTessellation(20.0f),
		m_minTessellation(0.0f),
		m_tessellationRange(100.0f),
		m_heightScale(1.0f)
	{
		auto renderer = Renderer::Create("builtin://renderer/tessellation.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		//renderStates->getRasterizerStateDesc()->FillMode = D3D11_FILL_WIREFRAME;
		renderStates->load(0);

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);


		auto sampler = std::make_shared<SamplerState>();
		sampler->load(0);
		auto texture = Texture::Create("builtin://texture/terrain/coast_sand_rocks_02_diff_2k.png");
		texture->load(0);

		setRenderPass(renderPass);
		createMesh();
		setConstant("u_maxTessellation", m_maxTessellation);
		setConstant("u_minTessellation", m_minTessellation);
		setConstant("u_tessellationRange", m_tessellationRange);
		setConstant("u_heightScale", m_heightScale);
		setSamplerSate("s_sampler", sampler);
		setShaderResource("t_albedo", texture);
	}

	void TessellationComponent::createMesh()
	{
		UINT m = 5;
		UINT n = 5;
		UINT width = 100;
		UINT depth = 100;

		UINT vertexCount = m * n;
		UINT faceCount = (m - 1) * (n - 1) * 2;

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;
		float dx = (float)width / (n - 1);
		float dz = (float)depth / (m - 1);
		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		std::shared_ptr<Blob> data = nullptr;
		std::vector<PositionNormalTexcoord> vertices;
		vertices.resize(vertexCount);

		for (UINT i = 0; i < m; ++i)
		{
			float z = halfDepth - i * dz;
			for (UINT j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * dx;
				vertices[i * n + j].position = DirectX::XMFLOAT3(x, 0.0f, z);
				vertices[i * n + j].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertices[i * n + j].texcoord = { j * du, i * dv };
			}
		}

		data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

		std::vector<unsigned short> indices;
		indices.resize(faceCount * 3);
		UINT k = 0;
		for (UINT i = 0; i < m - 1; ++i)
		{
			for (UINT j = 0; j < n - 1; ++j)
			{
				indices[k] = i * n + j;
				indices[k + 1] = i * n + j + 1;
				indices[k + 2] = (i + 1) * n + j;
				indices[k + 3] = (i + 1) * n + j;
				indices[k + 4] = i * n + j + 1;
				indices[k + 5] = (i + 1) * n + j + 1;
				k += 6; // next quad
			}
		}


		data.reset(new Blob(indices.size() * sizeof(unsigned short)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

		DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };

		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
		drawCall.vertexCount = (unsigned int)vertices.size();
		drawCall.indexCount = (unsigned int)indices.size();

		setMesh(std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer));
		getMesh()->load(0);
	}

	void TessellationComponent::set_maxTessellation(float maxTessellation)
	{
		m_maxTessellation = maxTessellation;
		setConstant("u_maxTessellation", m_maxTessellation);
	}

	void TessellationComponent::set_minTessellation(float minTessellation)
	{
		m_minTessellation = minTessellation;
		setConstant("u_minTessellation", m_minTessellation);
	}

	void TessellationComponent::set_tessellationRange(float tessellationRange)
	{
		m_tessellationRange = tessellationRange;
		setConstant("u_tessellationRange", m_tessellationRange);
	}

	void TessellationComponent::set_heightScale(float heightScale)
	{
		m_heightScale = heightScale;
		setConstant("u_heightScale", m_heightScale);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TessellationComponent>("TessellationComponent")
			.constructor<>()
			.property("maxTessellation", &TessellationComponent::get_maxTessellation, &TessellationComponent::set_maxTessellation)
			.property("minTessellation", &TessellationComponent::get_minTessellation, &TessellationComponent::set_minTessellation)
			.property("tessellationRange", &TessellationComponent::get_tessellationRange, &TessellationComponent::set_tessellationRange)
			.property("heightScale", &TessellationComponent::get_heightScale, &TessellationComponent::set_heightScale);
	}
}