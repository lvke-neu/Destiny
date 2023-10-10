#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "RenderWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
    m_renderWindow = new RenderWindow(this);
    setCentralWidget(m_renderWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_renderWindow;
}

