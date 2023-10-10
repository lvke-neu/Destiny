#pragma once
#include <QtWidgets/QApplication>
#include <memory>

class RenderWindow;
class Application : public QApplication
{
public:
	Application(int argc = 0, char* argv[] = nullptr);
private:
	std::shared_ptr<RenderWindow> m_renderWindow;
};

	