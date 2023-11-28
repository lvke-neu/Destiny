#include "Camera3DComponent.h"
#include "Node3D.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	Camera3DComponent::Camera3DComponent() : 
		m_fovy(60.0f),
		m_aspect(1.0f),
		m_nearz(1.0f),
		m_farz(1000.0f)
	{
		m_viewMatrix = std::make_shared<ConstantBuffer<XMMATRIX>>();
		m_projMatrix = std::make_shared<ConstantBuffer<XMMATRIX>>();
		
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&Camera3DComponent::onWindowResize, this, std::placeholders::_1));
	}

	Camera3DComponent::~Camera3DComponent()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&Camera3DComponent::onWindowResize, this, std::placeholders::_1));
	}

	void Camera3DComponent::onAttachNode()
	{
		m_viewMatrix->update(XMMatrixTranspose(XMMatrixInverse(nullptr, m_node->get_transform3D().getWorldMatrix())));
		m_projMatrix->update(XMMatrixTranspose(XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_nearz, m_farz)));
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(0, 1, m_viewMatrix->getConstantBuffer());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(1, 1, m_projMatrix->getConstantBuffer());
	}

	void Camera3DComponent::onNodeTransformChanged()
	{
		m_viewMatrix->update(XMMatrixTranspose(XMMatrixInverse(nullptr, m_node->get_transform3D().getWorldMatrix())));
	}

	void Camera3DComponent::set_fovy(float fovy)
	{
		m_fovy = fovy;
		m_projMatrix->update(XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovy), m_aspect, m_nearz, m_farz)));
	}

	void Camera3DComponent::set_aspect(float aspect)
	{
		m_aspect = aspect;
		m_projMatrix->update(XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovy), m_aspect, m_nearz, m_farz)));
	}

	void Camera3DComponent::set_nearz(float nearz)
	{
		m_nearz = nearz;
		m_projMatrix->update(XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovy), m_aspect, m_nearz, m_farz)));
	}

	void Camera3DComponent::set_farz(float farz)
	{
		m_farz = farz;
		m_projMatrix->update(XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovy), m_aspect, m_nearz, m_farz)));
	}

	void Camera3DComponent::onWindowResize(void* data)
	{
		WindowResizeData ws = *(WindowResizeData*)(data);
		m_aspect = static_cast<float>(ws.width) / ws.height;

		m_projMatrix->update(XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovy), m_aspect, m_nearz, m_farz)));
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Camera3DComponent>("Camera3DComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
			.property("fovy", &Camera3DComponent::get_fovy, &Camera3DComponent::set_fovy)
			.property("aspect", &Camera3DComponent::get_aspect, &Camera3DComponent::set_aspect)
			.property("nearz", &Camera3DComponent::get_nearz, &Camera3DComponent::set_nearz)
			.property("farz", &Camera3DComponent::get_farz, &Camera3DComponent::set_farz);
	}
}