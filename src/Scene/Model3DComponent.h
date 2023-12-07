#pragma once 
#include "Component.h"

namespace Destiny
{
	class Model3D;
	class Model3DComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		Model3DComponent();
	public:
		virtual void onAttachNode() override;
	private:
		std::shared_ptr<Model3D> m_model3D;
	};
}