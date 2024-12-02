#include "GeoSceneCuller.h"
#include "Scene/Geo/GeoScene.h"
#include "Scene/NCS/CameraComponent.h"
#include "Scene/NCS//VisualComponent.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Mesh.h"
#include <DirectXCollision.h>
#include <queue>

namespace Destiny
{
	GeoSceneCuller::GeoSceneCuller(std::shared_ptr<GeoScene> geoScene) :
		m_geoScene(geoScene)
	{

	}

	void GeoSceneCuller::onCull()
	{
		if (!m_geoScene || !m_geoScene->getCamera() || !m_geoScene->getCameraNode())
		{
			return;
		}

		DirectX::BoundingFrustum cameraFrustum;
		DirectX::BoundingFrustum::CreateFromMatrix(cameraFrustum, m_geoScene->getCamera()->getProjectionMatrix());
		cameraFrustum.Transform(cameraFrustum, m_geoScene->getCameraNode()->get_transform().getWorldMatrix());

		std::queue<std::shared_ptr<Node>> nodes;
		nodes.push(m_geoScene);
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
