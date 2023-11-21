#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "RenderWindow.h"


//using namespace Destiny;
//
//static void trace(QTreeWidgetItem* rootQtNode, std::shared_ptr<Node3D> rootNode3D)
//{
//	QTreeWidgetItem* node = new QTreeWidgetItem(rootQtNode);
//	node->setText(0, rootNode3D->m_name.c_str());
//
//	for (auto noded : rootNode3D->m_childs)
//	{
//		trace(node, noded);
//	}
//}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
    m_renderWindow = new RenderWindow(this);
    setCentralWidget(m_renderWindow);

 //   ui->treeWidget->setHeaderLabel({ "Node" });

	//std::shared_ptr<Node3D> root = std::make_shared<Node3D>();
	//root->m_name = "root";

	//std::shared_ptr<Node3D> n1 = std::make_shared<Node3D>();
	//n1->m_name = "n1";
	//n1->addToParent(root);
	//std::shared_ptr<Node3D> n11 = std::make_shared<Node3D>();
	//n11->m_name = "n11";
	//n11->addToParent(n1);
	//std::shared_ptr<Node3D> n12 = std::make_shared<Node3D>();
	//n12->m_name = "n12";
	//n12->addToParent(n1);


	//std::shared_ptr<Node3D> n2 = std::make_shared<Node3D>();
	//n2->m_name = "n2";
	//n2->addToParent(root);

	//QTreeWidgetItem* node = new QTreeWidgetItem(ui->treeWidget);
	//node->setText(0, root->m_name.c_str());
 //   trace(node, root);

	
	m_menu = new QMenu(ui->dockWidget);
	QAction* ac1 = new QAction("ac1", m_menu);
	QAction* ac2 = new QAction("ac2", m_menu);
	QAction* ac3 = new QAction("ac3", m_menu);

	m_menu->addAction(ac1);
	m_menu->addAction(ac2);
	m_menu->addAction(ac3);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_renderWindow;
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event)
{
	m_menu->exec(QCursor::pos());
}

