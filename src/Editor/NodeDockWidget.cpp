#include "NodeDockWidget.h"
#include <QTreeView>
#include <QVBoxLayout>

NodeDockWidget::NodeDockWidget(const QString &title, QWidget *parent /*= nullptr*/) : QDockWidget(title, parent)
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout(widget);

	QTreeView* treeView = new QTreeView(widget);
	layout->addWidget(treeView);

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
}

NodeDockWidget::~NodeDockWidget()
{

}

void NodeDockWidget::contextMenuEvent(QContextMenuEvent* event)
{
	m_menu->exec(QCursor::pos());
}

