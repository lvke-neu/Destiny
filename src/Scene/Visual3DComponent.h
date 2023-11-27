#pragma once
#include "Component.h"

namespace Destiny
{
	class Visual3D;
	class Visual3DComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		Visual3DComponent();
	public:
		GET_CLASS_NAME(Visual3DComponent);
		GET(std::shared_ptr<Visual3D>, visual3D);
	protected:
		std::shared_ptr<Visual3D> m_visual3D;
	};
}