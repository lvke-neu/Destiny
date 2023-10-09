#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Engine/Engine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Destiny::Engine::GetInstance()->initialize();
}

MainWindow::~MainWindow()
{
    Destiny::Engine::GetInstance()->uninitialize();
    delete ui;
}

