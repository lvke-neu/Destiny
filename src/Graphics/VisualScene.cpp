#include "VisualScene.h"
#include "CameraComponent.h"
#include "CameraController.h"
#include "VisualComponent.h"
#include "DirectionLightComponent.h"
#include "Mesh.h"
#include "RenderSystem.h"
#include "BindRenderTargets.h"
#include "Engine/Engine.h"
#include <queue>
#include <DirectXCollision.h>

namespace Destiny
{
	VisualScene::VisualScene(const std::string& name) : 
		Scene(name),
		m_bindRenderTargets(nullptr),
		m_clearRenderTargets(nullptr)
	{

	}

	void VisualScene::initialize()
	{
		m_bindRenderTargets = std::make_shared<BindRenderTargetsOnResize>();
		m_clearRenderTargets = std::make_shared<ClearRenderTargets>(m_bindRenderTargets);

		m_camera = std::make_shared<CameraComponent>();
		m_cameraController = std::make_shared<CameraController>();
		m_cameraNode = std::make_shared<Node>("Camera");
		m_cameraNode->addComponent(m_camera);
		m_cameraNode->addComponent(m_cameraController);
		m_cameraNode->addToParent(shared_from_this());

		m_directionLight = std::make_shared<DirectionLightComponent>();
		m_directionLightNode = std::make_shared<Node>("DirectionLight");
		m_directionLightNode->addComponent(m_directionLight);
		m_directionLightNode->addToParent(shared_from_this());
	}

	void VisualScene::uninitialize()
	{

	}

	void VisualScene::update(float deltaTime)
	{
		std::queue<std::shared_ptr<Node>> nodes;
		nodes.push(shared_from_this());
		while (!nodes.empty())
		{
			auto topNode = nodes.front();
			nodes.pop();
			if (topNode)
			{
				for (const auto& component : topNode->getComponents())
				{
					if (component)
					{
						component->onUpdate(deltaTime);
					}
				}
				for (const auto& node : topNode->getChilds())
				{
					nodes.push(node);
				}
			}
		}
	}

	void VisualScene::onCull()
	{
		std::dynamic_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(m_clearRenderTargets);

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
				if (!visualComponent || !visualComponent->get_enable())
				{
					continue;
				}
				std::dynamic_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->commitVisual(visualComponent->getVisual());
				//if (visualComponent->getVisual() && visualComponent->getVisual()->getMesh())
				//{
				//	if (visualComponent->getVisual()->getMesh()->getDrawCall().drawMethod == Mesh::DrawMethod::DrawIndexedInstanced)
				//	{
				//		Engine::GetInstance()->getGraphicsSystem()->commitVisual(visualComponent->getVisual());
				//		continue;
				//	}

				//	auto visualAABB = visualComponent->getVisual()->getMesh()->getBoundingBox();
				//	visualAABB.Transform(visualAABB, topNode->get_transform().getWorldMatrix());
				//	if (cameraFrustum.Intersects(visualAABB))
				//	{
				//		Engine::GetInstance()->getGraphicsSystem()->commitVisual(visualComponent->getVisual());
				//	}
				//}
			}
			for (const auto& node : topNode->getChilds())
			{
				nodes.push(node);
			}
		}
	}
}