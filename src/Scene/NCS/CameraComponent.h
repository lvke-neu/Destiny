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
		GET_CLASS_NAME(CameraComponent);
		GET(float, fovy);
		GET(float, aspect);
		GET(float, nearz);
		GET(float, farz);
		GET(float, viewportWidth);
		GET(float, viewportHeight);
	public:
		virtual void onEnterScene();
		virtual void onNodeTransformChanged();
	public:
		DirectX::XMMATRIX getProjectionMatrix();
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
		float m_viewportWidth;
		float m_viewportHeight;
	};
}