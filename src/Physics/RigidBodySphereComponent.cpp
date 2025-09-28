#include "RigidBodySphereComponent.h"
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
	RigidBodySphereComponent::RigidBodySphereComponent() :
		m_sphereRadius(0.5f)
	{
		m_btCollisionShape = std::make_shared<btSphereShape>(m_sphereRadius);

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

	RigidBodySphereComponent::~RigidBodySphereComponent()
	{

	}

	void RigidBodySphereComponent::set_sphereRadius(float sphereRadius)
	{
		if (Engine::GetInstance()->getPhysicsSystem()->get_bSimulation())
		{
			return;
		}

		m_sphereRadius = sphereRadius;
		m_btCollisionShape = std::make_shared<btSphereShape>(m_sphereRadius);

		reConstructRigidBody();
		modifyMesh();
	}

	void RigidBodySphereComponent::modifyMesh()
	{
        auto mesh = getMesh();
        if (!mesh)
        {
            std::shared_ptr<Blob> data = nullptr;
            std::vector<Position3> vertices;

            const int latLines = 8;
            const int lonLines = 16;
            float radius = m_sphereRadius;


            int totalVertices = 2 + latLines * lonLines;
            vertices.resize(totalVertices);

            vertices[0].position = DirectX::XMFLOAT3(0.0f, radius, 0.0f);
            vertices[1].position = DirectX::XMFLOAT3(0.0f, -radius, 0.0f);


            int vertexIndex = 2;
            float latStep = DirectX::XM_PI / (latLines + 1);
            float lonStep = 2.0f * DirectX::XM_PI / lonLines;

            for (int lat = 1; lat <= latLines; ++lat)
            {
                float latitude = DirectX::XM_PIDIV2 - lat * latStep;
                float sinLat = sinf(latitude);
                float cosLat = cosf(latitude);

                for (int lon = 0; lon < lonLines; ++lon)
                {
                    float longitude = lon * lonStep;
                    float sinLon = sinf(longitude);
                    float cosLon = cosf(longitude);


                    vertices[vertexIndex].position = DirectX::XMFLOAT3(
                        radius * cosLat * cosLon,
                        radius * sinLat,
                        radius * cosLat * sinLon
                    );
                    vertexIndex++;
                }
            }

            data.reset(new Blob(vertices.size() * sizeof(Position3)));
            data->copyfrom(vertices.data(), vertices.size() * sizeof(Position3));
            std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(
                InputLayout::Create_Position3(),
                (unsigned int)sizeof(Position3),
                0,
                data
                );


            std::vector<unsigned short> indices;

            for (int lon = 0; lon < lonLines; ++lon)
            {
                indices.push_back(0);
                indices.push_back(2 + lon);

                for (int lat = 0; lat < latLines - 1; ++lat)
                {
                    int current = 2 + lat * lonLines + lon;
                    int next = 2 + (lat + 1) * lonLines + lon;
                    indices.push_back(current);
                    indices.push_back(next);
                }

                indices.push_back(2 + (latLines - 1) * lonLines + lon);
                indices.push_back(1);
            }

            for (int lat = 0; lat < latLines; ++lat)
            {
                for (int lon = 0; lon < lonLines; ++lon)
                {
                    int current = 2 + lat * lonLines + lon;
                    int next = 2 + lat * lonLines + ((lon + 1) % lonLines);
                    indices.push_back(current);
                    indices.push_back(next);
                }
            }


            data.reset(new Blob(indices.size() * sizeof(unsigned short)));
            data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
            std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(
                IndexBuffer::IndexType::Index16,
                data
                );


            Mesh::DrawCall drawCall;
            drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
            drawCall.primitiveTopology = Mesh::PrimitiveTopology::LineList;
            drawCall.vertexCount = (unsigned int)vertices.size();
            drawCall.indexCount = (unsigned int)indices.size();


            DirectX::BoundingBox box;
            box.Center = { 0.0f, 0.0f, 0.0f };
            box.Extents = { radius, radius, radius };


            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(box, drawCall, vertexBuffer, indexBuffer);
            mesh->load(0);
            setMesh(mesh);
        }
        else
        {
            std::shared_ptr<Blob> data = nullptr;
            std::vector<Position3> vertices;

            const int latLines = 8;
            const int lonLines = 16;
            float radius = m_sphereRadius;


            int totalVertices = 2 + latLines * lonLines;
            vertices.resize(totalVertices);

            vertices[0].position = DirectX::XMFLOAT3(0.0f, radius, 0.0f);
            vertices[1].position = DirectX::XMFLOAT3(0.0f, -radius, 0.0f);


            int vertexIndex = 2;
            float latStep = DirectX::XM_PI / (latLines + 1);
            float lonStep = 2.0f * DirectX::XM_PI / lonLines;

            for (int lat = 1; lat <= latLines; ++lat)
            {
                float latitude = DirectX::XM_PIDIV2 - lat * latStep;
                float sinLat = sinf(latitude);
                float cosLat = cosf(latitude);

                for (int lon = 0; lon < lonLines; ++lon)
                {
                    float longitude = lon * lonStep;
                    float sinLon = sinf(longitude);
                    float cosLon = cosf(longitude);


                    vertices[vertexIndex].position = DirectX::XMFLOAT3(
                        radius * cosLat * cosLon,
                        radius * sinLat,
                        radius * cosLat * sinLon
                    );
                    vertexIndex++;
                }
            }

            data.reset(new Blob(vertices.size() * sizeof(Position3)));
            data->copyfrom(vertices.data(), vertices.size() * sizeof(Position3));
            std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(
                InputLayout::Create_Position3(),
                (unsigned int)sizeof(Position3),
                0,
                data
                );


            std::vector<unsigned short> indices;

            for (int lon = 0; lon < lonLines; ++lon)
            {
                indices.push_back(0);
                indices.push_back(2 + lon);

                for (int lat = 0; lat < latLines - 1; ++lat)
                {
                    int current = 2 + lat * lonLines + lon;
                    int next = 2 + (lat + 1) * lonLines + lon;
                    indices.push_back(current);
                    indices.push_back(next);
                }

                indices.push_back(2 + (latLines - 1) * lonLines + lon);
                indices.push_back(1);
            }

            for (int lat = 0; lat < latLines; ++lat)
            {
                for (int lon = 0; lon < lonLines; ++lon)
                {
                    int current = 2 + lat * lonLines + lon;
                    int next = 2 + lat * lonLines + ((lon + 1) % lonLines);
                    indices.push_back(current);
                    indices.push_back(next);
                }
            }


            data.reset(new Blob(indices.size() * sizeof(unsigned short)));
            data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
            std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(
                IndexBuffer::IndexType::Index16,
                data
                );


            Mesh::DrawCall drawCall;
            drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
            drawCall.primitiveTopology = Mesh::PrimitiveTopology::LineList;
            drawCall.vertexCount = (unsigned int)vertices.size();
            drawCall.indexCount = (unsigned int)indices.size();


            DirectX::BoundingBox box;
            box.Center = { 0.0f, 0.0f, 0.0f };
            box.Extents = { radius, radius, radius };

            mesh->modifyVertexBuffer(vertexBuffer);
            mesh->modifyBoundingBox(box);
        }
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RigidBodySphereComponent>("RigidBodySphereComponent")
			.constructor<>()
			.property("sphereRadius", &RigidBodySphereComponent::get_sphereRadius, &RigidBodySphereComponent::set_sphereRadius);
	}
}