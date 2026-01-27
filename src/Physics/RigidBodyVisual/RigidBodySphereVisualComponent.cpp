#include "RigidBodySphereVisualComponent.h"
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
#include <DirectXMath.h>
#include <vector>

namespace Destiny
{
	RigidBodySphereVisualComponent::RigidBodySphereVisualComponent()
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

	RigidBodySphereVisualComponent::~RigidBodySphereVisualComponent()
	{

	}

	void RigidBodySphereVisualComponent::modifyMesh(float radius)
	{
		auto mesh = getMesh();
		
		float visualRadius = radius * 1.01f;

		const int segments = 36;
		const float angleStep = DirectX::XM_2PI / segments;
		
		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<unsigned short> indices;

		// We need 3 circles * segments lines * 2 vertices per line = 216 vertices
		vertices.reserve(3 * segments * 2);
		indices.reserve(3 * segments * 2);

		// Helper to add a circle
		auto addCircle = [&](int axis) { 
			// axis: 0=XY, 1=YZ, 2=XZ
			for (int i = 0; i < segments; ++i)
			{
				float angle = i * angleStep;
				float nextAngle = (i + 1) * angleStep;
				
				float c1 = cos(angle) * visualRadius;
				float s1 = sin(angle) * visualRadius;
				float c2 = cos(nextAngle) * visualRadius;
				float s2 = sin(nextAngle) * visualRadius;

				DirectX::XMFLOAT3 p1, p2;

				if (axis == 0) // XY
				{
					p1 = { c1, s1, 0.0f };
					p2 = { c2, s2, 0.0f };
				}
				else if (axis == 1) // YZ
				{
					p1 = { 0.0f, c1, s1 };
					p2 = { 0.0f, c2, s2 };
				}
				else // XZ
				{
					p1 = { c1, 0.0f, s1 };
					p2 = { c2, 0.0f, s2 };
				}

				vertices.push_back(p1);
				vertices.push_back(p2);
			}
		};

		addCircle(0); // XY
		addCircle(1); // YZ
		addCircle(2); // XZ

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			indices.push_back((unsigned short)i);
		}

		if (!mesh)
		{
			std::shared_ptr<Blob> data = nullptr;

			data.reset(new Blob(vertices.size() * sizeof(DirectX::XMFLOAT3)));
			data->copyfrom(vertices.data(), vertices.size() * sizeof(DirectX::XMFLOAT3));
			std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(DirectX::XMFLOAT3), 0, data);

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
			box.Extents = { visualRadius, visualRadius, visualRadius };
			std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>(box, drawCall, vertexBuffer, indexBuffer);
			newMesh->load(0);
			setMesh(newMesh);
		}
		else
		{
			std::shared_ptr<Blob> data = nullptr;

			data.reset(new Blob(vertices.size() * sizeof(DirectX::XMFLOAT3)));
			data->copyfrom(vertices.data(), vertices.size() * sizeof(DirectX::XMFLOAT3));
			std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(DirectX::XMFLOAT3), 0, data);

			DirectX::BoundingBox box;
			box.Center = { 0.0f, 0.0f, 0.0f };
			box.Extents = { visualRadius, visualRadius, visualRadius };

			mesh->modifyVertexBuffer(vertexBuffer);
			mesh->modifyBoundingBox(box);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodySphereVisualComponent>("RigidBodySphereVisualComponent")
			.constructor<>();
	}
}
