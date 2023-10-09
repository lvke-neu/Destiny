#pragma once
#include <QtWidgets/QApplication>
#include <memory>

class MainWindow;
namespace Destiny
{
	class Application : public QApplication
	{
	public:
		Application(int argc = 0, char* argv[] = nullptr);
		~Application();
	private:
		std::shared_ptr<MainWindow> m_mainWindow;
	};
}
	