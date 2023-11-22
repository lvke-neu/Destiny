#pragma once
#include <QDockWidget>

class QMenu;
class QTreeWidget;
class QTreeWidgetItem;
namespace Destiny
{
	class Node3D;
}
class SceneDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	SceneDockWidget(QWidget *parent = nullptr);
	~SceneDockWidget();
public:
	virtual void contextMenuEvent(QContextMenuEvent* event) override;
signals:
	void chooseNode(QString uuid);
private:
	void trace(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, std::shared_ptr<Destiny::Node3D> parentNode);
private:
	QMenu* m_menu;
};


