#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class RenderWindow;
class SceneDockWidget;
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
	SceneDockWidget* m_sceneDockWidget;
	ComponentDockWidget* m_componentDockWidget;
};

#endif // MAINWINDOW_H
