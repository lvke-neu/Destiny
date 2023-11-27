#pragma once
#include <QDockWidget>
#include <memory>

namespace Destiny
{
	class Reflection;
	class Node3D;
}

class QVBoxLayout;
class QTableWidget;
class ComponentDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	ComponentDockWidget(QWidget *parent = nullptr);
	~ComponentDockWidget();
public slots:
	void onChooseNode(std::shared_ptr<Destiny::Node3D> node);
private:
	void reflect(std::shared_ptr<Destiny::Reflection> reflection, QTableWidget* tableWidget);
private:
	QVBoxLayout* m_layout;
	QTableWidget* m_tableWidget;
};


