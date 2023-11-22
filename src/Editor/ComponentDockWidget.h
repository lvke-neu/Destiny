#pragma once
#include <QDockWidget>

class ComponentDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	ComponentDockWidget(QWidget *parent = nullptr);
	~ComponentDockWidget();
public slots:
	void onChooseNode(QString uuid);
};


