#include "ComponentDockWidget.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>

ComponentDockWidget::ComponentDockWidget(QWidget *parent /*= nullptr*/) : QDockWidget("Component", parent)
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout(widget);

	QTableWidget* tableWidget = new QTableWidget(widget);
	tableWidget->setColumnCount(2);
	tableWidget->setRowCount(2);
	tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("key"));
	tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("value"));
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	//tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	layout->addWidget(tableWidget);
	
	tableWidget->setItem(0, 0, new QTableWidgetItem("hhh"));
	tableWidget->setCellWidget(0, 1, new QPushButton(tableWidget));

	tableWidget->setItem(1, 0, new QTableWidgetItem("hhh2"));
	tableWidget->setCellWidget(1, 1, new QCheckBox(tableWidget));

	this->setWidget(widget);

}

ComponentDockWidget::~ComponentDockWidget()
{

}

void ComponentDockWidget::onChooseNode(QString uuid)
{

}


