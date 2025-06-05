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

namespace Destiny
{
	TessellationComponent::TessellationComponent() :
		m_triEdgeTess0(1),
		m_triEdgeTess1(1),
		m_triEdgeTess2(1),
		m_triInsideTess(1)
	{
		auto renderer = Renderer::Create("builtin://renderer/tessellation.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getRasterizerStateDesc()->FillMode = D3D11_FILL_WIREFRAME;
		renderStates->load(0);

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		setRenderPass(renderPass);
		createMesh();
		setConstant("u_triEdgeTess0", m_triEdgeTess0);
		setConstant("u_triEdgeTess1", m_triEdgeTess1);
		setConstant("u_triEdgeTess2", m_triEdgeTess2);
		setConstant("u_triInsideTess", m_triInsideTess);
	}

	void TessellationComponent::createMesh()
	{
		std::shared_ptr<Blob> data = nullptr;
		std::vector<Position3> vertices;
		vertices.resize(3);

		vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);

		data.reset(new Blob(vertices.size() * sizeof(Position3)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(Position3));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(Position3), 0, data);

		DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };

		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::Draw;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
		drawCall.vertexCount = (unsigned int)vertices.size();

		setMesh(std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, nullptr));
		getMesh()->load(0);
	}

	void TessellationComponent::set_triEdgeTess0(unsigned int triEdgeTess0)
	{
		m_triEdgeTess0 = triEdgeTess0;
		setConstant("u_triEdgeTess0", m_triEdgeTess0);
	}

	void TessellationComponent::set_triEdgeTess1(unsigned int triEdgeTess1)
	{
		m_triEdgeTess1 = triEdgeTess1;
		setConstant("u_triEdgeTess1", m_triEdgeTess1);
	}

	void TessellationComponent::set_triEdgeTess2(unsigned int triEdgeTess2)
	{
		m_triEdgeTess2 = triEdgeTess2;
		setConstant("u_triEdgeTess2", m_triEdgeTess2);
	}

	void TessellationComponent::set_triInsideTess(unsigned int triInsideTess)
	{
		m_triInsideTess = triInsideTess;
		setConstant("u_triInsideTess", m_triInsideTess);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TessellationComponent>("TessellationComponent")
			.constructor<>()
			.property("triEdgeTess0", &TessellationComponent::get_triEdgeTess0, &TessellationComponent::set_triEdgeTess0)
			.property("triEdgeTess1", &TessellationComponent::get_triEdgeTess1, &TessellationComponent::set_triEdgeTess1)
			.property("triEdgeTess2", &TessellationComponent::get_triEdgeTess2, &TessellationComponent::set_triEdgeTess2)
			.property("triInsideTess", &TessellationComponent::get_triInsideTess, &TessellationComponent::set_triInsideTess);
	}
}