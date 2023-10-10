#include "RenderWindow.h"
#include "Engine/Engine.h"

RenderWindow::RenderWindow(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);

	Destiny::Engine::GetInstance()->initialize(winId(), width(), height());
}

RenderWindow::~RenderWindow()
{
	Destiny::Engine::GetInstance()->uninitialize();
}

void RenderWindow::paintEvent(QPaintEvent* event)
{
	Destiny::Engine::GetInstance()->update();
}

