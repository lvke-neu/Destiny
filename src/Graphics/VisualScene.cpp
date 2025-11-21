#include "VisualScene.h"
#include "CameraComponent.h"
#include "CameraController.h"
#include "VisualComponent.h"
#include "DirectionLightComponent.h"
#include "PointLightComponent.h"
#include "Mesh.h"
#include "RenderSystem.h"
#include "BindRenderTargets.h"
#include "Engine/Engine.h"
#include <queue>
#include <DirectXCollision.h>

namespace Destiny
{
	VisualScene::VisualScene() :
		Scene(),
		m_enableCull(true),
		m_intersectionCount(0)
	{

	}

	VisualScene::VisualScene(const std::string& name) :
		Scene(name),
		m_enableCull(true)
	{

	}

	void VisualScene::initialize()
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
					if (std::dynamic_pointer_cast<CameraComponent>(component))
					{
						m_camera = std::dynamic_pointer_cast<CameraComponent>(component);
						m_cameraNode = topNode;
					}
					if (std::dynamic_pointer_cast<CameraController>(component))
					{
						m_cameraController = std::dynamic_pointer_cast<CameraController>(component);
						m_cameraNode = topNode;
					}

					if (std::dynamic_pointer_cast<DirectionLightComponent>(component) || std::dynamic_pointer_cast<PointLightComponent>(component))
					{
						component->onEnterScene();
					}
				}
				for (const auto& node : topNode->getChilds())
				{
					nodes.push(node);
				}
			}
		}

		if (m_camera && m_cameraController && m_camera->get_node().lock() == m_cameraNode && m_cameraController->get_node().lock() == m_cameraNode)
		{
			m_camera->onEnterScene();
			return;
		}

		m_camera = std::make_shared<CameraComponent>();
		m_cameraController = std::make_shared<CameraController>();
		m_cameraNode = std::make_shared<Node>("Camera");
		m_cameraNode->addComponent(m_camera);
		m_cameraNode->addComponent(m_cameraController);
		m_cameraNode->addToParent(shared_from_this());

		m_camera->onEnterScene();
	}

	void VisualScene::uninitialize()
	{
		DirectX::XMMATRIX T
		(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f
		);

		Renderer::SetConstant("g_directionLightCount", 0);
		Renderer::SetConstant("g_pointLightCount", 0);
		Renderer::SetConstant("T", XMMatrixTranspose(T));
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
					if (component && component->get_updateCategory() == Component::UpdateCategory::update)
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

	void VisualScene::lateUpdate(float deltaTime)
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
					if (component && component->get_updateCategory() == Component::UpdateCategory::late_update)
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
		m_intersectionCount = 0;

		DirectX::BoundingFrustum cameraFrustum;
		DirectX::BoundingFrustum::CreateFromMatrix(cameraFrustum, m_camera->getProjectionMatrix());
		cameraFrustum.Transform(cameraFrustum, m_cameraNode->getWorldMatrix());

		std::queue<std::shared_ptr<Node>> nodes;
		nodes.push(shared_from_this());
		while (!nodes.empty())
		{
			auto topNode = nodes.front();
			nodes.pop();

			if (!topNode)
			{
				continue;
			}

			auto rootToThisWorldMatrix = topNode->getRootToThisWorldMatrix();
			for (const auto& component : topNode->getComponents())
			{
				auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
				if (!visualComponent || !visualComponent->get_enable())
				{
					continue;
				}

				if (!m_enableCull)
				{
					std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->commitVisual(visualComponent->getVisual());
					continue;
				}

				if (visualComponent->getVisual() && visualComponent->getVisual()->getMesh())
				{
					if (visualComponent->getVisual()->getMesh()->getDrawCall().drawMethod == Mesh::DrawMethod::DrawIndexedInstanced)
					{
						std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->commitVisual(visualComponent->getVisual());
						continue;
					}

					auto visualAABB = visualComponent->getVisual()->getMesh()->getBoundingBox();
					visualAABB.Transform(visualAABB, rootToThisWorldMatrix);
					if (cameraFrustum.Intersects(visualAABB))
					{
						std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->commitVisual(visualComponent->getShadowVisual());
						std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->commitVisual(visualComponent->getVisual());
					}

					++m_intersectionCount;
				}
			}



			//for (const auto& component : topNode->getComponents())
			//{
			//	auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
			//	if (!visualComponent || !visualComponent->get_enable())
			//	{
			//		continue;
			//	}
			//	std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->commitVisual(visualComponent->getVisual());
			//}
			for (const auto& node : topNode->getChilds())
			{
				nodes.push(node);
			}
		}
	}

	void VisualScene::locate(const Transform& transform)
	{
		if (m_cameraNode)
		{
			m_cameraNode->set_transform(transform);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualScene>("VisualScene")
			.constructor<>()
		.property("enableCull", &VisualScene::get_enableCull, &VisualScene::set_enableCull);
	}
}