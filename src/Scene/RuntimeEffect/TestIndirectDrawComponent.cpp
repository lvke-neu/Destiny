#include "TestIndirectDrawComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Mesh.h"
#include "Graphics/InputLayout.h"
#include "Graphics/VertexDefine.h"
#include "Graphics/IndirectBuffer.h"
#include "Engine/Blob.h"
#include "Math/Math.h"

namespace Destiny
{
	TestIndirectDrawComponent::TestIndirectDrawComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/forward_pbr.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = getIndirectMesh();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
		set_material(PbrMaterial::Create_Wall());

		//auto shadowRenderer = Renderer::Create("builtin://renderer/forward_pbr_shadow.hlsl");
		//shadowRenderer->load(0);
		//auto shadowRenderStates = std::make_shared<RenderStates>();
		//shadowRenderStates->load();
		//auto shadowRenderPass = std::make_shared<RenderPass>();
		//shadowRenderPass->setRenderer(shadowRenderer);
		//shadowRenderPass->setRenderStates(shadowRenderStates);

		//setShadowRenderPass(shadowRenderPass);
		//setShadowMesh(mesh);
	}

	std::shared_ptr<Mesh> TestIndirectDrawComponent::getIndirectMesh()
	{
		std::shared_ptr<Blob> data = nullptr;
		std::vector<PositionNormalTexcoord> vertices;
		vertices.resize(27);

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

		vertices[24].position = { -3.0f, 0.0f, 3.0f };
		vertices[24].normal = { 0.0f, 0.0f, -1.0f };
		vertices[24].texcoord = { 0.5f, 0.5f};
		vertices[25].position = { 0.0f, 3.0f, 3.0f };
		vertices[25].normal = { 0.0f, 0.0f, -1.0f };
		vertices[25].texcoord = { 0.5f, 0.5f };
		vertices[26].position = { 3.0f, 0.0f, 3.0f };
		vertices[26].normal = { 0.0f, 0.0f, -1.0f };
		vertices[26].texcoord = { 0.5f, 0.5f };

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
			20, 21, 22, 22, 23, 20,
			24,25,26
		};
		data.reset(new Blob(indices.size() * sizeof(unsigned short)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

		DirectX::BoundingBox aabb;
		DirectX::BoundingBox::CreateFromPoints(aabb, { -10.0f,-10.0f,-10.0f }, { 10.0f,10.0f,10.0f });

		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndirect;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;

		std::vector<D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS> indirectArgs;
		indirectArgs.resize(2);

		indirectArgs[0].IndexCountPerInstance = 36;
		indirectArgs[0].InstanceCount = 1;
		indirectArgs[0].StartIndexLocation = 0;
		indirectArgs[0].BaseVertexLocation = 0;
		indirectArgs[0].StartInstanceLocation = 0;

		indirectArgs[1].IndexCountPerInstance = 3;
		indirectArgs[1].InstanceCount = 1;
		indirectArgs[1].StartIndexLocation = 36;
		indirectArgs[1].BaseVertexLocation = 0;
		indirectArgs[1].StartInstanceLocation = 0;

		data.reset(new Blob(indirectArgs.size() * sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS)));
		data->copyfrom(indirectArgs.data(), data->getLength());

		std::shared_ptr<IndirectBuffer> indirectBuffer = std::make_shared<IndirectBuffer>(data);
		indirectBuffer->load(0);

		std::vector<std::pair<Mesh::DrawIndirectMethod, unsigned int>> drawIndirectMethod_indirectBufferOffsets;
		drawIndirectMethod_indirectBufferOffsets.resize(2);
		drawIndirectMethod_indirectBufferOffsets[0] = { Mesh::DrawIndirectMethod::DrawIndexedInstancedIndirect, 0 };
		drawIndirectMethod_indirectBufferOffsets[1] = { Mesh::DrawIndirectMethod::DrawIndexedInstancedIndirect, (unsigned int)sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS) };

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);
		mesh->setIndirectMode(indirectBuffer, drawIndirectMethod_indirectBufferOffsets);

		return mesh;
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TestIndirectDrawComponent>("TestIndirectDrawComponent")
			.constructor<>();
	}
}