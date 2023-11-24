#pragma once
#include <QDockWidget>
#include <memory>

namespace Destiny
{
	class Node3D;
}

class QVBoxLayout;
class ComponentDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	ComponentDockWidget(QWidget *parent = nullptr);
	~ComponentDockWidget();
public slots:
	void onChooseNode(std::shared_ptr<Destiny::Node3D> node);
private:
	QVBoxLayout* m_layout;
};


