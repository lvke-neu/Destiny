#include "Scene.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
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

	void Scene::initialize()
	{
		//camera
		m_cameraNode = std::make_shared<Node>("Camera");
		m_cameraNode->moveZAxis(-5.0f);
		m_camera = std::make_shared<CameraComponent>();
		m_cameraNode->addComponent(m_camera);
		m_cameraNode->addComponent(std::make_shared<CameraController>());
		m_cameraNode->addToParent(shared_from_this());

		testDrawIndexInstance();

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
			renderPass->setPipeline(RenderPass::ForwardOpaque);
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

	void Scene::testDrawIndexInstance()
	{

	}

	void Scene::uninitialize()
	{

	}

	void Scene::update(float deltaTime)
	{

	}
}