#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class RigidBodyBoxVisualComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		RigidBodyBoxVisualComponent();
		virtual ~RigidBodyBoxVisualComponent();
	public:
		void modifyMesh(const DirectX::XMFLOAT3& boxHalfExtents);
	};
}