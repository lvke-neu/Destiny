#include "RenderWindow.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include <qevent.h>

RenderWindow::RenderWindow(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);

	Destiny::EngineSetting setting{ (long long)winId(), 32 };
	Destiny::Engine::GetInstance()->initialize(setting);
	Destiny::Engine::GetInstance()->getEventSystem()->registerEvent(Destiny::EventType::Update, std::bind(&RenderWindow::onUpdate, this, std::placeholders::_1));
}

RenderWindow::~RenderWindow()
{
	Destiny::Engine::GetInstance()->uninitialize();
}

void RenderWindow::paintEvent(QPaintEvent* event)
{
	Destiny::Engine::GetInstance()->update();
	update();
}

void RenderWindow::resizeEvent(QResizeEvent* event)
{
	Destiny::WindowResizeData data{ (unsigned int)width(), (unsigned int)height() };
	Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::WindowResize, &data);
}

void RenderWindow::keyPressEvent(QKeyEvent* event)
{
	Destiny::KeyCode code = (Destiny::KeyCode)event->key();
	Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::KeyPressed, &code);
}

void RenderWindow::keyReleaseEvent(QKeyEvent* event)
{
	Destiny::KeyCode code = (Destiny::KeyCode)event->key();
	Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::KeyReleased, &code);
}

void RenderWindow::mousePressEvent(QMouseEvent* event)
{
	Destiny::Mouse mouse;
	mouse.code = (Destiny::MouseCode)event->button();
	mouse.x = event->pos().x();
	mouse.y = event->pos().y();
	Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MousePressed, &mouse);
}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
	Destiny::Mouse mouse;
	
	if (event->buttons() & Qt::RightButton)
	{
		mouse.code = (Destiny::MouseCode)Qt::RightButton;
	}

	if (event->buttons() & Qt::LeftButton)
	{
		mouse.code = (Destiny::MouseCode)Qt::LeftButton;
	}

	mouse.x = event->pos().x();
	mouse.y = event->pos().y();
	Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MouseMoved, &mouse);

	repaint();
}

void RenderWindow::mouseReleaseEvent(QMouseEvent* event)
{
	Destiny::Mouse mouse;
	mouse.code = (Destiny::MouseCode)event->button();
	mouse.x = event->pos().x();
	mouse.y = event->pos().y();
	Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MouseReleased, &mouse);
}


void RenderWindow::onUpdate(void* data)
{
	static float sumTime = 0.0f;
	sumTime += *(float*)(data);
	if (sumTime > 1)
	{
		float fps = 1 / *(float*)(data);
		parentWidget()->setWindowTitle(("FPS:" + std::to_string(fps)).c_str());
		sumTime = 0.0f;
	}
}
