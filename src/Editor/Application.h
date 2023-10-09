#pragma once
#include <QtWidgets/QApplication>
#include <memory>

class MainWindow;
class Application : public QApplication
{
public:
	Application(int argc = 0, char* argv[] = nullptr);
private:
	std::shared_ptr<MainWindow> m_mainWindow;
};

	