#include "RigidBodyBoxComponent.h"
#include "Engine/Node.h"
#include "Engine/Engine.h"
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
#include "Physics/PhysicsSystem.h"
#include <btBulletDynamicsCommon.h>

namespace Destiny
{
	RigidBodyBoxComponent::RigidBodyBoxComponent() :
		m_boxHalfExtents({ 0.5f, 0.5f, 0.5f })
	{
		m_btCollisionShape = std::make_shared<btBoxShape>(btVector3(m_boxHalfExtents.x, m_boxHalfExtents.y, m_boxHalfExtents.z));
	
		auto renderer = Renderer::Create("builtin://renderer/box_visualization.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Gui);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		setRenderPass(renderPass);
	}

	RigidBodyBoxComponent::~RigidBodyBoxComponent()
	{

	}

	void RigidBodyBoxComponent::set_boxHalfExtents(DirectX::XMFLOAT3 boxHalfExtents)
	{
		if (Engine::GetInstance()->getPhysicsSystem()->get_bSimulation())
		{
			return;
		}

		m_boxHalfExtents = boxHalfExtents;
		m_btCollisionShape = std::make_shared<btBoxShape>(btVector3(boxHalfExtents.x, boxHalfExtents.y, boxHalfExtents.z));
		
		reConstructRigidBody();
		modifyMesh();
	}

	void RigidBodyBoxComponent::modifyMesh()
	{
		auto mesh = getMesh();
		if (!mesh)
		{
			std::shared_ptr<Blob> data = nullptr;
			std::vector<Position3> vertices;
			vertices.resize(8);

			DirectX::XMFLOAT3 min = { -m_boxHalfExtents.x, -m_boxHalfExtents.y, -m_boxHalfExtents.z };
			DirectX::XMFLOAT3 max = { m_boxHalfExtents.x, m_boxHalfExtents.y, m_boxHalfExtents.z };

			vertices[0].position = DirectX::XMFLOAT3(min.x, min.y, min.z);
			vertices[1].position = DirectX::XMFLOAT3(max.x, min.y, min.z);
			vertices[2].position = DirectX::XMFLOAT3(max.x, max.y, min.z);
			vertices[3].position = DirectX::XMFLOAT3(min.x, max.y, min.z);
			vertices[4].position = DirectX::XMFLOAT3(min.x, min.y, max.z);
			vertices[5].position = DirectX::XMFLOAT3(max.x, min.y, max.z);
			vertices[6].position = DirectX::XMFLOAT3(max.x, max.y, max.z);
			vertices[7].position = DirectX::XMFLOAT3(min.x, max.y, max.z);


			data.reset(new Blob(vertices.size() * sizeof(Position3)));
			data->copyfrom(vertices.data(), vertices.size() * sizeof(Position3));
			std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(Position3), 0, data);

			std::vector<unsigned short> indices =
			{
				0, 1, 1, 2, 2, 3, 3, 0,
				4, 5, 5, 6, 6, 7, 7, 4,
				0, 4, 1, 5, 2, 6, 3, 7
			};

			data.reset(new Blob(indices.size() * sizeof(unsigned short)));
			data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
			std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

			Mesh::DrawCall drawCall;
			drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
			drawCall.primitiveTopology = Mesh::PrimitiveTopology::LineList;
			drawCall.vertexCount = (unsigned int)vertices.size();
			drawCall.indexCount = (unsigned int)indices.size();


			DirectX::BoundingBox box;
			box.Center = { 0.0f, 0.0f, 0.0f };
			box.Extents = { m_boxHalfExtents.x, m_boxHalfExtents.y, m_boxHalfExtents.z };
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(box, drawCall, vertexBuffer, indexBuffer);
			mesh->load(0);
			setMesh(mesh);
		}
		else
		{
			std::shared_ptr<Blob> data = nullptr;
			std::vector<Position3> vertices;
			vertices.resize(8);

			DirectX::XMFLOAT3 min = { -m_boxHalfExtents.x, -m_boxHalfExtents.y, -m_boxHalfExtents.z };
			DirectX::XMFLOAT3 max = { m_boxHalfExtents.x, m_boxHalfExtents.y, m_boxHalfExtents.z };

			vertices[0].position = DirectX::XMFLOAT3(min.x, min.y, min.z);
			vertices[1].position = DirectX::XMFLOAT3(max.x, min.y, min.z);
			vertices[2].position = DirectX::XMFLOAT3(max.x, max.y, min.z);
			vertices[3].position = DirectX::XMFLOAT3(min.x, max.y, min.z);
			vertices[4].position = DirectX::XMFLOAT3(min.x, min.y, max.z);
			vertices[5].position = DirectX::XMFLOAT3(max.x, min.y, max.z);
			vertices[6].position = DirectX::XMFLOAT3(max.x, max.y, max.z);
			vertices[7].position = DirectX::XMFLOAT3(min.x, max.y, max.z);


			data.reset(new Blob(vertices.size() * sizeof(Position3)));
			data->copyfrom(vertices.data(), vertices.size() * sizeof(Position3));
			std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(Position3), 0, data);

			DirectX::BoundingBox box;
			box.Center = { 0.0f, 0.0f, 0.0f };
			box.Extents = {m_boxHalfExtents.x, m_boxHalfExtents.y, m_boxHalfExtents.z };

			mesh->modifyVertexBuffer(vertexBuffer);
			mesh->modifyBoundingBox(box);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodyBoxComponent>("RigidBodyBoxComponent")
			.constructor<>()
			.property("boxHalfExtents", &RigidBodyBoxComponent::get_boxHalfExtents, &RigidBodyBoxComponent::set_boxHalfExtents);
	}
}