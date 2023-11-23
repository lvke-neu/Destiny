#pragma once
#include <QDockWidget>
#include <memory>

class Node3D;
class ComponentDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	ComponentDockWidget(QWidget *parent = nullptr);
	~ComponentDockWidget();
public slots:
	void onChooseNode(std::shared_ptr<Node3D> node);
};


