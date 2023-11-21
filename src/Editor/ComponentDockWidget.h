#pragma once
#include <QDockWidget>
#include <QMenu>

class ComponentDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	ComponentDockWidget(QWidget *parent = nullptr);
	~ComponentDockWidget();
};


