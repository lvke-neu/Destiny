#include "CameraController.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Node.h"

namespace Destiny
{
	CameraController::CameraController() :
		m_speed(5.0f),
		m_oldMousePosX(0.0f),
		m_oldMousePosY(0.0f),
		m_deltaMousePosX(0.0f),
		m_deltaMousePosY(0.0f)
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::Update, std::bind(&CameraController::onUpdateTime, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::MousePressed, std::bind(&CameraController::onMousePress, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::MouseReleased, std::bind(&CameraController::onMouseRelease, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::MouseMoved, std::bind(&CameraController::onMouseMove, this, std::placeholders::_1));
	}

	CameraController::~CameraController()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::Update, std::bind(&CameraController::onUpdateTime, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::MousePressed, std::bind(&CameraController::onMousePress, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::MouseReleased, std::bind(&CameraController::onMouseRelease, this, std::placeholders::_1));
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::MouseMoved, std::bind(&CameraController::onMouseMove, this, std::placeholders::_1));
	}

	void CameraController::onUpdateTime(void* data)
	{
		if (!m_node)
		{
			return;
		}

		float deltaTime = *(float*)(data);

		if (Engine::GetInstance()->getEventSystem()->isKeyPressed(KeyCode::Key_W))
		{
			m_node->moveZAxis(m_speed * deltaTime);
		}

		if (Engine::GetInstance()->getEventSystem()->isKeyPressed(KeyCode::Key_S))
		{
			m_node->moveZAxis(-m_speed * deltaTime);
		}

		if (Engine::GetInstance()->getEventSystem()->isKeyPressed(KeyCode::Key_A))
		{
			m_node->moveXAxis(-m_speed * deltaTime);
		}

		if (Engine::GetInstance()->getEventSystem()->isKeyPressed(KeyCode::Key_D))
		{
			m_node->moveXAxis(m_speed * deltaTime);
		}
	}

	void CameraController::onMousePress(void* data)
	{
		Mouse mouse = *(Mouse*)data;

		if (mouse.code == MouseCode::RightButton)
		{
			m_oldMousePosX = mouse.x;
			m_oldMousePosY = mouse.y;
		}
	}

	void CameraController::onMouseRelease(void* data)
	{
		Mouse mouse = *(Mouse*)data;

		if (mouse.code == MouseCode::RightButton)
		{
			m_deltaMousePosX = 0;
			m_deltaMousePosY = 0;
		}
	}

	void CameraController::onMouseMove(void* data)
	{
		if (!m_node)
		{
			return;
		}

		Mouse mouse = *(Mouse*)data;

		if (mouse.code == MouseCode::RightButton)
		{
			m_deltaMousePosX = mouse.x - m_oldMousePosX;
			m_deltaMousePosY = mouse.y - m_oldMousePosY;

			m_node->rotateXAxis(-m_deltaMousePosY * 0.1f);
			m_node->rotateYAxis(-m_deltaMousePosX * 0.1f);

			m_oldMousePosX = mouse.x;
			m_oldMousePosY = mouse.y;
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<CameraController>("CameraController")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
			.property("speed", &CameraController::get_speed, &CameraController::set_speed);
	}
}