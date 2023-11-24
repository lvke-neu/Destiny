#pragma once
#include <QDockWidget>
#include <memory>
#include "Scene/Node3D.h"

class QMenu;
class QTreeWidget;
class QTreeWidgetItem;

Q_DECLARE_METATYPE(std::shared_ptr<Destiny::Node3D>);

class SceneDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	SceneDockWidget(QWidget *parent = nullptr);
	~SceneDockWidget();
public:
	virtual void contextMenuEvent(QContextMenuEvent* event) override;
signals:
	void chooseNode(std::shared_ptr<Destiny::Node3D> node);
private:
	void trace(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, std::shared_ptr<Destiny::Node3D> parentNode);
private:
	QMenu* m_menu;
};


