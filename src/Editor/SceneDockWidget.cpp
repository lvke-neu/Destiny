#include "SceneDockWidget.h"
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QTreeWidgetItem>

SceneDockWidget::SceneDockWidget(QWidget *parent /*= nullptr*/) : QDockWidget("Scene", parent)
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout(widget);

	QTreeWidget* treeWidget = new QTreeWidget(widget);
	treeWidget->setHeaderLabel("Node");
	layout->addWidget(treeWidget);

	this->setWidget(widget);

	m_menu = new QMenu(this);
	QAction* ac1 = new QAction("添加", m_menu);
	QAction* ac2 = new QAction("删除", m_menu);
	QAction* ac3 = new QAction("复制", m_menu);
	QAction* ac4 = new QAction("粘贴", m_menu);

	m_menu->addAction(ac1);
	m_menu->addAction(ac2);
	m_menu->addAction(ac3);
	m_menu->addAction(ac4);

	QTreeWidgetItem* root = new QTreeWidgetItem(treeWidget);
	root->setText(0, "root");

	QTreeWidgetItem* node1 = new QTreeWidgetItem(root);
	node1->setText(0, "node1");
	QTreeWidgetItem* node11 = new QTreeWidgetItem(node1);
	node11->setText(0, "node11");
	QTreeWidgetItem* node12 = new QTreeWidgetItem(node1);
	node12->setText(0, "node12");
	QTreeWidgetItem* node13 = new QTreeWidgetItem(node1);
	node13->setText(0, "node1");

	QTreeWidgetItem* node2 = new QTreeWidgetItem(root);
	node2->setText(0, "node2");

	QTreeWidgetItem* node3 = new QTreeWidgetItem(root);
	node3->setText(0, "node3");
}

SceneDockWidget::~SceneDockWidget()
{

}

void SceneDockWidget::contextMenuEvent(QContextMenuEvent* event)
{
	m_menu->exec(QCursor::pos());
}

