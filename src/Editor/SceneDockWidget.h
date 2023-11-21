#pragma once
#include <QDockWidget>
#include <QMenu>

class SceneDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	SceneDockWidget(QWidget *parent = nullptr);
	~SceneDockWidget();
public:
	virtual void contextMenuEvent(QContextMenuEvent* event) override;
private:
	QMenu* m_menu;
};


