#include "ComponentDockWidget.h"
#include <QLabel>
#include <QVBoxLayout>

ComponentDockWidget::ComponentDockWidget(QWidget *parent /*= nullptr*/) : QDockWidget("Component", parent)
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout(widget);

	m_label = new QLabel("null", widget);
	layout->addWidget(m_label);
	
	this->setWidget(widget);

}

ComponentDockWidget::~ComponentDockWidget()
{

}

void ComponentDockWidget::onChooseNode(QString uuid)
{
	m_label->setText(uuid);
}


