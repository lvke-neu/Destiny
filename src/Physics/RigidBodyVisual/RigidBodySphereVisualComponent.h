#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class RigidBodySphereVisualComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		RigidBodySphereVisualComponent();
		virtual ~RigidBodySphereVisualComponent();
	public:
		void modifyMesh(float radius);
	};
}
