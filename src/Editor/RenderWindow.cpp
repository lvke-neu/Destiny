#include "RenderWindow.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"

RenderWindow::RenderWindow(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);

	Destiny::Engine::GetInstance()->initialize(winId());
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
