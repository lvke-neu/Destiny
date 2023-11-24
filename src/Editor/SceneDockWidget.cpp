#include "SceneDockWidget.h"
#include <QMenu>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include "Engine/Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene3D.h"

using namespace Destiny;

void SceneDockWidget::trace(QTreeWidget* treeWidget,QTreeWidgetItem* parentItem, std::shared_ptr<Node3D> parentNode)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
	item->setText(0, parentNode->get_name().c_str());
	item->setData(0, 1, QVariant::fromValue(parentNode));
	connect(treeWidget, &QTreeWidget::itemClicked, this,
		[this](QTreeWidgetItem* item)
		{
			emit chooseNode(item->data(0, 1).value<std::shared_ptr<Destiny::Node3D>>());
		});

	for (const auto& node : parentNode->get_childs())
	{
		trace(treeWidget, item, node);
	}
}

SceneDockWidget::SceneDockWidget(QWidget *parent /*= nullptr*/) : QDockWidget("Scene3D", parent)
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout(widget);

	QTreeWidget* treeWidget = new QTreeWidget(widget);
	treeWidget->setHeaderLabel("Node3D");
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

	auto rootNode = Engine::GetInstance()->getSceneManager()->getScene3D()->getRootNode();

	QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
	rootItem->setText(0, rootNode->get_name().c_str());
	rootItem->setData(0, 1, QVariant::fromValue(rootNode));
	connect(treeWidget, &QTreeWidget::itemClicked, this,
		[this](QTreeWidgetItem* item)
		{
			emit chooseNode(item->data(0, 1).value<std::shared_ptr<Destiny::Node3D>>());
		});

	for (const auto& node : rootNode->get_childs())
	{
		trace(treeWidget, rootItem, node);
	}

}

SceneDockWidget::~SceneDockWidget()
{

}

void SceneDockWidget::contextMenuEvent(QContextMenuEvent* event)
{
	m_menu->exec(QCursor::pos());
}

