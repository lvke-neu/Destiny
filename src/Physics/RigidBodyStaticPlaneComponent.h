#pragma once
#include "RigidBodyComponent.h"

namespace Destiny
{
	class RigidBodyStaticPlaneComponent : public RigidBodyComponent
	{
		RTTR_ENABLE(RigidBodyComponent);
	public:
		RigidBodyStaticPlaneComponent();
		virtual ~RigidBodyStaticPlaneComponent();
	public:
		GET(float, position);
		void set_position(float position);
	private:
		float m_position;
	};
}