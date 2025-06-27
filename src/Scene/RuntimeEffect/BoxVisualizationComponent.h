#pragma once
#include "Graphics/VisualComponent.h"
#include <DirectXCollision.h>

namespace Destiny
{
	class BoxVisualizationComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		BoxVisualizationComponent();
		virtual ~BoxVisualizationComponent() = default;
	public:
		void modifyMesh(const DirectX::BoundingBox& aabb);
	};
}