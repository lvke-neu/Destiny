#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class CameraController : public Component
	{
		RTTR_ENABLE(Component);
	public:
		CameraController();
		virtual ~CameraController();
	private:
		void onUpdateTime(void* data);
		void onMousePress(void* data);
		void onMouseRelease(void* data);
		void onMouseMove(void* data);
	public:
		GET_CLASS_NAME(CameraController);
		GET_SET(float, speed);
	private:
		float m_speed;

		float m_oldMousePosX;
		float m_oldMousePosY;
		float m_deltaMousePosX;
		float m_deltaMousePosY;
	};
}