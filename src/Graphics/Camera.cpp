#include "Camera.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include <DirectXMath.h>
#include <d3d11.h>

namespace Destiny
{
	struct ConstantDataView
	{
		DirectX::XMMATRIX view;
	};
	struct ConstantDataProj
	{
		DirectX::XMMATRIX proj;
	};
	Camera::Camera(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		m_constantBufferView = std::make_unique<ConstantBuffer>(device, sizeof(ConstantDataView));
		m_constantBufferProj = std::make_unique<ConstantBuffer>(device, sizeof(ConstantDataProj));
		context->VSSetConstantBuffers(0, 1, &m_constantBufferView->m_pConstantBuffer);
		context->VSSetConstantBuffers(1, 1, &m_constantBufferProj->m_pConstantBuffer);
		m_transform = new Transform({ 1,1,1 }, {0,0,0}, {0,0,-5});
		m_context = context;

		updateViewMat();

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&Camera::onResize, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::Update, std::bind(&Camera::onUpdate, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::MousePressed, std::bind(&Camera::onMousePress, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::MouseReleased, std::bind(&Camera::onMouseRelease, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::MouseMoved, std::bind(&Camera::onMouseMove, this, std::placeholders::_1));
	}

	Camera::~Camera()
	{
		delete m_transform;
	}

	void Camera::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		ConstantDataProj proj;
		proj.proj = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, (float)wrd.width / wrd.height, 1.0f, 1000.0f));
		m_constantBufferProj->updateData(m_context, &proj, sizeof(proj));
	}	
	void Camera::updateViewMat()
	{
		ConstantDataView view;
		auto ppp = m_transform->getWorldMatrix();
		view.view = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform->getWorldMatrix()));
		m_constantBufferView->updateData(m_context, &view, sizeof(view));
	}
	void Camera::onUpdate(void* data)
	{
		float deltaTime = *(float*)data;
		bool bMove = false;
		float speed = 10;
		if (Engine::GetInstance()->getEventSystem()->isKeyPressed(KeyCode::Key_W))
		{
			m_transform->moveZAxis(deltaTime * speed);
			bMove = true;
		}
		if (Engine::GetInstance()->getEventSystem()->isKeyPressed(KeyCode::Key_S))
		{
			m_transform->moveZAxis(-deltaTime * speed);
			bMove = true;
		}
		if (Engine::GetInstance()->getEventSystem()->isKeyPressed(KeyCode::Key_A))
		{
			m_transform->moveXAxis(-deltaTime * speed);
			bMove = true;
		}
		if (Engine::GetInstance()->getEventSystem()->isKeyPressed(KeyCode::Key_D))
		{
			m_transform->moveXAxis(deltaTime * speed);
			bMove = true;
		}

		if (bMove)
		{
			updateViewMat();
		}
	}

	void Camera::onMousePress(void* data)
	{
		Mouse mouse = *(Mouse*)data;

		if (mouse.code == MouseCode::RightButton)
		{
			m_oldMousePosX = mouse.x;
			m_oldMousePosY = mouse.y;
		}
	}

	void Camera::onMouseRelease(void* data)
	{
		Mouse mouse = *(Mouse*)data;

		if (mouse.code == MouseCode::RightButton)
		{
			m_deltaMousePosX = 0;
			m_deltaMousePosY = 0;
		}
	}

	void Camera::onMouseMove(void* data)
	{
		Mouse mouse = *(Mouse*)data;

		if (mouse.code == MouseCode::RightButton)
		{
			m_deltaMousePosX = mouse.x - m_oldMousePosX;
			m_deltaMousePosY = mouse.y - m_oldMousePosY;

			m_transform->rotXAxis(m_deltaMousePosY * 0.01f);
			m_transform->rotYAxis(m_deltaMousePosX * 0.01f);
			updateViewMat();

			m_oldMousePosX = mouse.x;
			m_oldMousePosY = mouse.y;
		}
	}

}