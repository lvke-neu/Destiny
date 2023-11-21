#include "ComponentDockWidget.h"
#include <QPushButton>
#include <QVBoxLayout>

ComponentDockWidget::ComponentDockWidget(const QString &title, QWidget *parent /*= nullptr*/) : QDockWidget(title, parent)
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

