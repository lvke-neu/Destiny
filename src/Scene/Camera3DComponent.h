#pragma once
#include "Component.h"
#include "Graphics/ConstantBuffer.h"
#include <memory>
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;
	class Camera3DComponent : public Component
	{
		RTTR_ENABLE(Reflection);
	public:
		Camera3DComponent();
		~Camera3DComponent();
	public:
		virtual void onAttachNode() override;
		virtual void onNodeTransformChanged() override;
	public:
		GET_CLASS_NAME(Camera3DComponent);
		GET(float, fovy);
		GET(float, aspect);
		GET(float, nearz);
		GET(float, farz);

		void set_fovy(float fovy);
		void set_aspect(float aspect);
		void set_nearz(float nearz);
		void set_farz(float farz);
	private:
		void onWindowResize(void* data);
	private:
		float m_fovy;
		float m_aspect;
		float m_nearz;
		float m_farz;

		std::shared_ptr<ConstantBuffer<XMMATRIX>> m_viewMatrix;
		std::shared_ptr<ConstantBuffer<XMMATRIX>> m_projMatrix;
	};
}