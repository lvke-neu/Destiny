#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class RenderWindow;
class NodeDockWidget;
class ComponentDockWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    RenderWindow* m_renderWindow;
	NodeDockWidget* m_nodeDockWidget;
	ComponentDockWidget* m_componentDockWidget;
};

#endif // MAINWINDOW_H
