#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class PlaneComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		PlaneComponent();
	public:
		GET_CLASS_NAME(PlaneComponent);
	private:
		int m_width;
		int m_height;
	};
}