#pragma once
#include <QDockWidget>
#include <QMenu>

class ComponentDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	ComponentDockWidget(const QString &title, QWidget *parent = nullptr);
	~ComponentDockWidget();
};


