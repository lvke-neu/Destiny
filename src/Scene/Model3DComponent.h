#pragma once 
#include "Component.h"

namespace Destiny
{
	class Model3D;
	class Model3DComponent : public Component, public std::enable_shared_from_this<Model3DComponent>
	{
		RTTR_ENABLE(Component);
	public:
		Model3DComponent();
	public:
		GET(std::string, path);

		void set_path(std::string path);
	private:
		std::shared_ptr<Model3D> m_model3D;
		std::string m_path;
	};
}