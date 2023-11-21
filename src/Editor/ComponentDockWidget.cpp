#include "ComponentDockWidget.h"
#include <QPushButton>
#include <QVBoxLayout>

ComponentDockWidget::ComponentDockWidget(QWidget *parent /*= nullptr*/) : QDockWidget("Component", parent)
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout(widget);

	QPushButton* button = new QPushButton("add component", widget);
	layout->addWidget(button);

	this->setWidget(widget);

}

ComponentDockWidget::~ComponentDockWidget()
{

}

