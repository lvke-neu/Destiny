#pragma once
#include "Engine/Component.h"
#include <DirectXMath.h>

namespace Destiny
{
	class CameraComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		CameraComponent();
		virtual ~CameraComponent();
	public:
		GET(float, fovy);
		void set_fovy(float fovy);
		GET(float, aspect);
		void set_aspect(float aspect);
		GET(float, nearz);
		void set_nearz(float nearz);
		GET(float, farz);
		void set_farz(float farz);
		GET(float, viewportWidth);
		GET(float, viewportHeight);
	public:
		virtual void			onEnterScene() override;
		virtual void			onNodeTransformChanged() override;
	public:
		DirectX::XMMATRIX		getProjectionMatrix();
	private:
		void					onWindowResize(void* data);
	private:
		void					notifyVisualRendererConstantChanged(std::shared_ptr<Node> node);
		void					setRendererConstant();
	private:
		float					m_fovy;
		float					m_aspect;
		float					m_nearz;
		float					m_farz;
		float					m_viewportWidth;
		float					m_viewportHeight;
	};
}