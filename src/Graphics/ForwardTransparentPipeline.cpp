#include "ForwardTransparentPipeline.h"
#include "Visual.h"
#include "Mesh.h"
#include "Engine/Component.h"
#include "VisualScene.h"
#include <algorithm>

namespace Destiny
{
	ForwardTransparentPipeline::ForwardTransparentPipeline() 
	{
		
	}

	ForwardTransparentPipeline::~ForwardTransparentPipeline()
	{
		
	}

	void ForwardTransparentPipeline::execute(ID3D11DeviceContext* deviceContext)
	{
		sort();
		GraphicsCommandList::execute(deviceContext);
	}

	void ForwardTransparentPipeline::sort()
	{
		for (auto& graphicsCommandMap : m_graphicsCommandList)
		{
			std::vector<std::shared_ptr<GraphicsCommand>> graphicsCommands(graphicsCommandMap.second.begin(), graphicsCommandMap.second.end());
			std::sort(graphicsCommands.begin(), graphicsCommands.end(),
				[](std::shared_ptr<GraphicsCommand> a, std::shared_ptr<GraphicsCommand> b)->bool
				{
					if (!a || !b || a == b)
					{
						return false;
					}

					auto visual_a = std::dynamic_pointer_cast<Visual>(a);
					auto visual_b = std::dynamic_pointer_cast<Visual>(b);
					if (!visual_a || !visual_b || !visual_a->getComponent() || !visual_b->getComponent())
					{
						return false;
					}

					auto node_a = visual_a->getComponent()->get_node();
					auto node_b = visual_b->getComponent()->get_node();
					if (!node_a || !node_b)
					{
						return false;
					}

					auto mesh_a = visual_a->getMesh();
					auto mesh_b = visual_b->getMesh();
					if (!mesh_a || !mesh_b)
					{
						return false;
					}

					auto aabb_a = mesh_a->getBoundingBox();
					auto aabb_b = mesh_b->getBoundingBox();

					aabb_a.Transform(aabb_a, node_a->get_transform().getWorldMatrix());
					aabb_b.Transform(aabb_b, node_b->get_transform().getWorldMatrix());

					auto scene_a = std::dynamic_pointer_cast<VisualScene>(visual_a->getComponent()->get_scene());
					auto scene_b = std::dynamic_pointer_cast<VisualScene>(visual_b->getComponent()->get_scene());
					if (!scene_a || !scene_b || (scene_a != scene_b))
					{
						return false;
					}

					auto cameraPosition = scene_a->getCameraNode()->get_transform().get_translation();
					auto distance_square_a =
						std::pow(cameraPosition.x - aabb_a.Center.x, 2) +
						std::pow(cameraPosition.y - aabb_a.Center.y, 2) +
						std::pow(cameraPosition.z - aabb_a.Center.z, 2);
					auto distance_square_b =
						std::pow(cameraPosition.x - aabb_b.Center.x, 2) +
						std::pow(cameraPosition.y - aabb_b.Center.y, 2) +
						std::pow(cameraPosition.z - aabb_b.Center.z, 2);
					return distance_square_a > distance_square_b;
				});
			graphicsCommandMap.second = { graphicsCommands.begin(), graphicsCommands.end() };
		}
	}
}