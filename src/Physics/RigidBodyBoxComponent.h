#pragma once
#include "RigidBodyComponent.h"

namespace Destiny
{
	class RigidBodyBoxComponent : public RigidBodyComponent
	{
		RTTR_ENABLE(RigidBodyComponent);
	public:
		RigidBodyBoxComponent();
		virtual ~RigidBodyBoxComponent();
	public:
		GET(DirectX::XMFLOAT3, boxHalfExtents);
		void set_boxHalfExtents(DirectX::XMFLOAT3 boxHalfExtents);
	private:
		void modifyMesh();
	private:
		DirectX::XMFLOAT3 m_boxHalfExtents;
	};
}