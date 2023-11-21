#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class RenderWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void contextMenuEvent(QContextMenuEvent* event) override;
private:
    Ui::MainWindow *ui;
    RenderWindow* m_renderWindow;
    QMenu* m_menu;
};

#endif // MAINWINDOW_H
