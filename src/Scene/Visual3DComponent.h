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
		void set_visual3D(std::shared_ptr<Visual3D> visual3D)
		{
			m_visual3D.reset();
			m_visual3D = visual3D;
		}
	private:
		std::shared_ptr<Visual3D> m_visual3D;
	};
}