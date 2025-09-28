#pragma once
#include "RigidBodyComponent.h"

namespace Destiny
{
	class RigidBodySphereComponent : public RigidBodyComponent
	{
		RTTR_ENABLE(RigidBodyComponent);
	public:
		RigidBodySphereComponent();
		virtual ~RigidBodySphereComponent();
	public:
		GET(float, sphereRadius);
		void set_sphereRadius(float sphereRadius);
	private:
		void modifyMesh();
	private:
		float m_sphereRadius;
	};
}