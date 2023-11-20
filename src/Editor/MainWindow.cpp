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

    ui->treeWidget->setHeaderLabel({ "Node" });
    QTreeWidgetItem* node = new QTreeWidgetItem(ui->treeWidget);
    node->setText(0, "aa");
    QTreeWidgetItem* node2 = new QTreeWidgetItem(node);
    node2->setText(0, "bb");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_renderWindow;
}

