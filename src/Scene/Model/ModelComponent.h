#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class Model;
	class ModelComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		ModelComponent();
		virtual ~ModelComponent();
	public:
		GET_CLASS_NAME(ModelComponent);
	public:
		GET(std::string, path);
		void set_path(const std::string& path);
	private:
		virtual void onUpdate(float deltaTime) override;
	private:
		std::string				m_path;
		std::shared_ptr<Model>	m_model;
		bool					m_modelChanged;
	};
}