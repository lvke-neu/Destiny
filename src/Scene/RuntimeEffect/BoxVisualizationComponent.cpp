#include "BoxVisualizationComponent.h"
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
	BoxVisualizationComponent::BoxVisualizationComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/box_visualization.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Gui);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		setRenderPass(renderPass);

		modifyMesh({ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f,1.0f } });
	}

	void BoxVisualizationComponent::modifyMesh(const DirectX::BoundingBox& aabb)
	{
		auto mesh = getMesh();
		if (!mesh)
		{
			std::shared_ptr<Blob> data = nullptr;
			std::vector<Position3> vertices;
			vertices.resize(8);
		
			DirectX::XMFLOAT3 min = { aabb.Center.x - aabb.Extents.x, aabb.Center.y - aabb.Extents.y, aabb.Center.z - aabb.Extents.z };
			DirectX::XMFLOAT3 max = { aabb.Center.x + aabb.Extents.x, aabb.Center.y + aabb.Extents.y, aabb.Center.z + aabb.Extents.z };

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

			DirectX::BoundingBox aabb;
			DirectX::BoundingBox::CreateFromPoints(aabb, { -FLT_MAX,-FLT_MAX,-FLT_MAX }, { FLT_MAX,FLT_MAX,FLT_MAX });

			Mesh::DrawCall drawCall;
			drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
			drawCall.primitiveTopology = Mesh::PrimitiveTopology::LineList;
			drawCall.vertexCount = (unsigned int)vertices.size();
			drawCall.indexCount = (unsigned int)indices.size();

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);
			mesh->load(0);
			setMesh(mesh);
		}
		else
		{
			std::shared_ptr<Blob> data = nullptr;
			std::vector<Position3> vertices;
			vertices.resize(8);

			DirectX::XMFLOAT3 min = { aabb.Center.x - aabb.Extents.x, aabb.Center.y - aabb.Extents.y, aabb.Center.z - aabb.Extents.z };
			DirectX::XMFLOAT3 max = { aabb.Center.x + aabb.Extents.x, aabb.Center.y + aabb.Extents.y, aabb.Center.z + aabb.Extents.z };

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

			mesh->modifyVertexBuffer(vertexBuffer);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BoxVisualizationComponent>("BoxVisualizationComponent")
			.constructor<>();
	}
}