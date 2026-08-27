#include "RigidBodyBoxVisualComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"

#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/FileSystem.h"
#include "Engine/Md5.h"
#include "Graphics/Texture.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"


namespace Destiny
{
	RigidBodyBoxVisualComponent::RigidBodyBoxVisualComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/rigid_body_shape.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Gui);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		setRenderPass(renderPass);
	}

	RigidBodyBoxVisualComponent::~RigidBodyBoxVisualComponent()
	{

	}

	void RigidBodyBoxVisualComponent::modifyMesh(const DirectX::XMFLOAT3& boxHalfExtents)
	{
		DirectX::XMFLOAT3 visualBoxHalfExtents = 
		{ 
			boxHalfExtents.x * 1.01f, 
			boxHalfExtents.y * 1.01f, 
			boxHalfExtents.z * 1.01f 
		};

		auto mesh = getMesh();
		if (!mesh)
		{
			std::shared_ptr<Blob> data = nullptr;
			std::vector<DirectX::XMFLOAT3> vertices;
			vertices.resize(8);

			DirectX::XMFLOAT3 min = { -visualBoxHalfExtents.x, -visualBoxHalfExtents.y, -visualBoxHalfExtents.z };
			DirectX::XMFLOAT3 max = { visualBoxHalfExtents.x, visualBoxHalfExtents.y, visualBoxHalfExtents.z };

			vertices[0] = DirectX::XMFLOAT3(min.x, min.y, min.z);
			vertices[1] = DirectX::XMFLOAT3(max.x, min.y, min.z);
			vertices[2] = DirectX::XMFLOAT3(max.x, max.y, min.z);
			vertices[3] = DirectX::XMFLOAT3(min.x, max.y, min.z);
			vertices[4] = DirectX::XMFLOAT3(min.x, min.y, max.z);
			vertices[5] = DirectX::XMFLOAT3(max.x, min.y, max.z);
			vertices[6] = DirectX::XMFLOAT3(max.x, max.y, max.z);
			vertices[7] = DirectX::XMFLOAT3(min.x, max.y, max.z);


			data.reset(new Blob(vertices.size() * sizeof(DirectX::XMFLOAT3)));
			data->copyfrom(vertices.data(), vertices.size() * sizeof(DirectX::XMFLOAT3));
			std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(DirectX::XMFLOAT3), 0, data);

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
			box.Extents = { visualBoxHalfExtents.x, visualBoxHalfExtents.y, visualBoxHalfExtents.z };
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(box, drawCall, vertexBuffer, indexBuffer);
			mesh->load(0);
			setMesh(mesh);
		}
		else
		{
			std::shared_ptr<Blob> data = nullptr;
			std::vector<DirectX::XMFLOAT3> vertices;
			vertices.resize(8);

			DirectX::XMFLOAT3 min = { -visualBoxHalfExtents.x, -visualBoxHalfExtents.y, -visualBoxHalfExtents.z };
			DirectX::XMFLOAT3 max = { visualBoxHalfExtents.x, visualBoxHalfExtents.y, visualBoxHalfExtents.z };

			vertices[0] = DirectX::XMFLOAT3(min.x, min.y, min.z);
			vertices[1] = DirectX::XMFLOAT3(max.x, min.y, min.z);
			vertices[2] = DirectX::XMFLOAT3(max.x, max.y, min.z);
			vertices[3] = DirectX::XMFLOAT3(min.x, max.y, min.z);
			vertices[4] = DirectX::XMFLOAT3(min.x, min.y, max.z);
			vertices[5] = DirectX::XMFLOAT3(max.x, min.y, max.z);
			vertices[6] = DirectX::XMFLOAT3(max.x, max.y, max.z);
			vertices[7] = DirectX::XMFLOAT3(min.x, max.y, max.z);


			data.reset(new Blob(vertices.size() * sizeof(DirectX::XMFLOAT3)));
			data->copyfrom(vertices.data(), vertices.size() * sizeof(DirectX::XMFLOAT3));
			std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(DirectX::XMFLOAT3), 0, data);

			DirectX::BoundingBox box;
			box.Center = { 0.0f, 0.0f, 0.0f };
			box.Extents = { visualBoxHalfExtents.x, visualBoxHalfExtents.y, visualBoxHalfExtents.z };

			mesh->modifyVertexBuffer(vertexBuffer);
			mesh->modifyBoundingBox(box);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodyBoxVisualComponent>("RigidBodyBoxVisualComponent")
			.constructor<>();
	}
}