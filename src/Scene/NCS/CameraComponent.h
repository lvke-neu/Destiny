#pragma once
#include "Component.h"

namespace Destiny
{
	class CameraComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		CameraComponent();
		~CameraComponent();
	public:
		GET_CLASS_NAME(Component);
		GET(float, fovy);
		GET(float, aspect);
		GET(float, nearz);
		GET(float, farz);
	public:
		virtual void onEnterScene(std::shared_ptr<Scene> scene);
		virtual void onNodeTransformChanged(const Transform& transform);
	private:
		void onWindowResize(void* data);
	private:
		void bfsNotifyViewChanged(std::shared_ptr<Node> node);
		void bfsNotifyProjChanged(std::shared_ptr<Node> node);
	private:
		float m_fovy;
		float m_aspect;
		float m_nearz;
		float m_farz;
	};
}