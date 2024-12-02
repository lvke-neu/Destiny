#include "GeoScene.h"
#include "../NCS/VisualComponent.h"
#include "../NCS/CameraComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/GraphicsSystem.h"
#include <queue>

namespace Destiny
{
	GeoScene::GeoScene(const std::string& name) : Scene(name)
	{

	}

	GeoScene::~GeoScene()
	{

	}

	void GeoScene::initialize()
	{
		Scene::initialize();

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

			auto mesh = MeshProvider::Create_Plane_PositionNormalTexcoord();
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

		////box
		//{
		//	auto renderer = std::make_shared<Renderer>("builtin://renderer/basic.rdr");
		//	renderer->load(0);
		//	renderer->setConstant("u_color", DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f });

		//	std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		//	renderStates->load();

		//	std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		//	renderPass->setRendererCategory(RenderPass::ForwardOpaque);
		//	renderPass->setRenderer(renderer);
		//	renderPass->setRenderStates(renderStates);

		//	auto mesh = Mesh::Create_Box_PositionNormalTexcoord();
		//	mesh->load();

		//	std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
		//	visualComponent->setRenderPass(renderPass);
		//	visualComponent->setMesh(mesh);

		//	auto node = std::make_shared<Node>();
		//	node->set_name("BoxNode");
		//	node->addComponent(visualComponent);
		//	node->addToParent(shared_from_this());
		//}
	}

	void GeoScene::uninitialize()
	{

	}

	void GeoScene::update(float deltaTime)
	{

	}

	void GeoScene::onCull()
	{
		if (!m_camera || !m_cameraNode)
		{
			return;
		}

		DirectX::BoundingFrustum cameraFrustum;
		DirectX::BoundingFrustum::CreateFromMatrix(cameraFrustum, m_camera->getProjectionMatrix());
		cameraFrustum.Transform(cameraFrustum, m_cameraNode->get_transform().getWorldMatrix());
		
		std::queue<std::shared_ptr<Node>> nodes; 
		nodes.push(shared_from_this()); 
		while (!nodes.empty())
		{
			auto topNode = nodes.front(); 
			nodes.pop(); 
			for (const auto& component : topNode->getComponents())
			{
				auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
				if (visualComponent && visualComponent->getVisual() && visualComponent->getVisual()->getMesh())
				{
					if (visualComponent->getVisual()->getMesh()->getDrawCall().drawMethod == Mesh::DrawMethod::DrawIndexedInstanced)
					{
						Engine::GetInstance()->getGraphicsSystem()->commitVisual(visualComponent->getVisual());
						continue;
					}

					auto visualAABB = visualComponent->getVisual()->getMesh()->getBoundingBox();
					visualAABB.Transform(visualAABB, topNode->get_transform().getWorldMatrix());
					if (cameraFrustum.Intersects(visualAABB))
					{
						Engine::GetInstance()->getGraphicsSystem()->commitVisual(visualComponent->getVisual());
					}	
				}
			}
			for (const auto& node : topNode->getChilds())
			{
				nodes.push(node); 
			}
		}
	}
}