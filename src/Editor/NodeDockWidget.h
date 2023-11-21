#pragma once
#include <QDockWidget>
#include <QMenu>

class NodeDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	NodeDockWidget(const QString &title, QWidget *parent = nullptr);
	~NodeDockWidget();
public:
	virtual void contextMenuEvent(QContextMenuEvent* event) override;
private:
	QMenu* m_menu;
};


