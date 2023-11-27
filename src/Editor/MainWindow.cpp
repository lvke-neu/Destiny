#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "RenderWindow.h"
#include "SceneDockWidget.h"
#include "ComponentDockWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  

    m_renderWindow = new RenderWindow(this);
    this->setCentralWidget(m_renderWindow);

	m_sceneDockWidget = new SceneDockWidget(this);
	this->addDockWidget(Qt::LeftDockWidgetArea, m_sceneDockWidget);

	m_componentDockWidget = new ComponentDockWidget(this);
	this->addDockWidget(Qt::RightDockWidgetArea, m_componentDockWidget);

	connect(m_sceneDockWidget, &SceneDockWidget::chooseNode, m_componentDockWidget, &ComponentDockWidget::onChooseNode);

	//this->setWindowState(Qt::WindowMaximized);
	//this->setFixedSize(300, 300);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_renderWindow;
	delete m_sceneDockWidget;
	delete m_componentDockWidget;
}

