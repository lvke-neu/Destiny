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
		virtual ~Model3DComponent();
	public:
		GET_CLASS_NAME(Model3DComponent);
	public:
		virtual void onNodeTransformChanged() override;
	public:
		GET(std::string, path);
		GET(bool, show);
		void set_path(std::string path);
		void set_show(bool show);
	private:
		void update(void* data);
	private:
		std::shared_ptr<Model3D> m_model3D;
		std::string m_path;
		bool m_show;
	};
}