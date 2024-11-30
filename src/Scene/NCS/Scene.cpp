#include "Scene.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/InstanceBuffer.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Visual.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include "VisualComponent.h"
#include "CameraComponent.h"
#include "CameraController.h"

#include "Engine/EventSystem.h"
#include <d3d11.h>

namespace Destiny
{
	Scene::Scene() : Node("Root")
	{
		
	}

	Scene::~Scene()
	{

	}

	static int drawcallcount = 6000;
	void Scene::initialize()
	{
		//camera
		m_cameraNode = std::make_shared<Node>("Camera");
		m_cameraNode->moveZAxis(-5.0f);
		m_camera = std::make_shared<CameraComponent>();
		m_cameraNode->addComponent(m_camera);
		m_cameraNode->addComponent(std::make_shared<CameraController>());
		m_cameraNode->addToParent(shared_from_this());

		testDrawIndex();
		//testDrawIndexInstance();

		//for (int i = 0; i < 1; i++)
		//{
		//	//box
		//	{
		//		auto renderer = std::make_shared<Renderer>("builtin://renderer/basic.rdr");
		//		renderer->load(0);
		//		renderer->setConstant("u_color", DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f });

		//		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		//		renderStates->load();

		//		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		//		renderPass->setPipeline(RenderPass::ForwardOpaque);
		//		renderPass->setRenderer(renderer);
		//		renderPass->setRenderStates(renderStates);

		//		auto mesh = Mesh::Create_Box_PositionNormalTexcoord();
		//		mesh->load();

		//		std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
		//		visualComponent->setRenderPass(renderPass);
		//		visualComponent->setMesh(mesh);

		//		auto node = std::make_shared<Node>();
		//		node->set_name("BoxNode");
		//		node->addComponent(visualComponent);
		//		node->addToParent(shared_from_this());
		//	}
		//}


		//plane
		{
			auto renderer = std::make_shared<Renderer>("builtin://renderer/basic.rdr");
			renderer->load(0);
			renderer->setConstant("u_color", DirectX::XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });

			std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
			renderStates->load();

			std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
			renderPass->setRendererCategory(RenderPass::ForwardOpaque);
			renderPass->setRenderer(renderer);
			renderPass->setRenderStates(renderStates);

			auto mesh = Mesh::Create_Plane_PositionNormalTexcoord();
			mesh->load();

			std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
			visualComponent->setRenderPass(renderPass);
			visualComponent->setMesh(mesh);

			auto node = std::make_shared<Node>();
			node->set_name("PlaneNode");
			node->addComponent(visualComponent);
			node->addToParent(shared_from_this());
			Transform transform;
			transform.set_scale({ 10.0f, 10.0f, 1.0f });
			transform.set_rotation({ 90.0f, 0.0f, 0.0f });
			transform.set_translation({ 0.0f, -3.0f, 0.0f });
			node->set_transform(transform);
		}

	}

	void Scene::testDrawIndex()
	{
		for (int i = 0; i < drawcallcount; i++)
		{
			auto renderer = std::make_shared<Renderer>("builtin://renderer/basic.rdr");
			renderer->load(0);
			renderer->setConstant("u_color", DirectX::XMFLOAT4{ (float)(i%2),1.0f,(float)(i % 2), 1.0f });

			std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
			renderStates->load();

			std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
			renderPass->setRendererCategory(RenderPass::ForwardOpaque);
			renderPass->setRenderer(renderer);
			renderPass->setRenderStates(renderStates);

			auto mesh = Mesh::Create_Box_PositionNormalTexcoord();
			mesh->load();

			std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
			visualComponent->setRenderPass(renderPass);
			visualComponent->setMesh(mesh);

			auto node = std::make_shared<Node>();
			node->set_name("BoxNode");
			node->addComponent(visualComponent);
			node->addToParent(shared_from_this());

			Transform transform;
			transform.set_translation({ (float)i, 0.0f, 0.0f });
			node->set_transform(transform);
		}
	}

	void Scene::testDrawIndexInstance()
	{
		struct PositionNormalTexcoord
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 texcoord;
		};


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
		for (UINT i = 0; i < 4; ++i)
		{
			vertices[i].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[i + 4].normal = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertices[i + 8].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[i + 12].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[i + 16].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertices[i + 20].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		}
		for (UINT i = 0; i < 6; ++i)
		{
			vertices[i * 4].texcoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[i * 4 + 1].texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			vertices[i * 4 + 2].texcoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[i * 4 + 3].texcoord = DirectX::XMFLOAT2(1.0f, 1.0f);

		}
		data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			
			{ "WORLDMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "WORLDMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "WORLDMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "WORLDMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};

		std::shared_ptr<Blob> inputLayoutDesc = std::make_shared<Blob>(sizeof(D3D11_INPUT_ELEMENT_DESC) * inputElements.size());
		inputLayoutDesc->copyfrom(inputElements.data(), sizeof(D3D11_INPUT_ELEMENT_DESC)* inputElements.size());

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(std::make_shared<InputLayout>(inputLayoutDesc), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);

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

		DirectX::BoundingBox aabb({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f });
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexedInstanced;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		struct InstanceData
		{
			DirectX::XMMATRIX worldMatrix;
			DirectX::XMFLOAT4 color;
		};
		std::vector<InstanceData> instanceDatas;
		instanceDatas.resize(drawcallcount);
		Transform transform;
		for (int i = 0; i < instanceDatas.size(); i++)
		{
			auto r = i % 2;
			instanceDatas[i].color = { (float)r,1.0f,(float)r, 1.0f };
			transform.set_translation({ (float)i, 0.0f, 0.0f });
			instanceDatas[i].worldMatrix = transform.getTransposeWorldMatrix();
		}


		data.reset(new Blob((unsigned int)(instanceDatas.size() * sizeof(InstanceData))));
		data->copyfrom(instanceDatas.data(), (unsigned int)(instanceDatas.size() * sizeof(InstanceData)));

		std::shared_ptr<InstanceBuffer> instanceBuffer = std::make_shared<InstanceBuffer>((unsigned int)(instanceDatas.size() * sizeof(InstanceData)), (unsigned int)sizeof(InstanceData), 0, (unsigned int)instanceDatas.size());
		instanceBuffer->updateInstanceData(data);

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer, instanceBuffer);
		mesh->load(0);

		auto renderer = std::make_shared<Renderer>("builtin://renderer/basic_instance.rdr");
		renderer->load(0);
		
		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RenderPass::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
		visualComponent->setRenderPass(renderPass);
		visualComponent->setMesh(mesh);

		auto node = std::make_shared<Node>();
		node->set_name("BoxInstanceNode");
		node->addComponent(visualComponent);
		node->addToParent(shared_from_this());
	}

	void Scene::uninitialize()
	{

	}

	void Scene::update(float deltaTime)
	{

	}
}