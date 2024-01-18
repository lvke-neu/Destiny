#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class ParticleComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		ParticleComponent();
	public:
		GET_CLASS_NAME(ParticleComponent);
	private:
		void update(void* data);
	public:
		float m_sumDeltaTime;
		float m_initY;
	};
}