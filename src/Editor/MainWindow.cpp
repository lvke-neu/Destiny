#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "RenderWindow.h"
#include "NodeDockWidget.h"
#include "ComponentDockWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  

    m_renderWindow = new RenderWindow(this);
    this->setCentralWidget(m_renderWindow);

	m_nodeDockWidget = new NodeDockWidget("Node", this);
	this->addDockWidget(Qt::LeftDockWidgetArea, m_nodeDockWidget);

	m_componentDockWidget = new ComponentDockWidget("Component", this);
	this->addDockWidget(Qt::RightDockWidgetArea, m_componentDockWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_renderWindow;
	delete m_nodeDockWidget;
	delete m_componentDockWidget;
}

