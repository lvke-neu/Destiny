#pragma once
#include <QtWidgets/QApplication>
#include <memory>

class MainWindow;
namespace Destiny
{
	class RenderView
	{
	public:
		RenderView(int argc = 0, char* argv[] = nullptr);
		~RenderView();
	public:
		void initialize();
		void uninitialize();
		void run();
	private:
		QApplication m_application;
		std::shared_ptr<MainWindow> m_mainWindow;
	};
}