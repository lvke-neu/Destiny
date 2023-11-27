#include "ComponentDockWidget.h"
#include "Scene/Node3D.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <rttr/type>

ComponentDockWidget::ComponentDockWidget(QWidget *parent /*= nullptr*/) : QDockWidget("Component", parent)
{
	QWidget* widget = new QWidget();
	m_layout = new QVBoxLayout(widget);

	m_tableWidget = new QTableWidget(widget);
	m_tableWidget->horizontalHeader()->setStretchLastSection(true);
	m_layout->addWidget(m_tableWidget);

	this->setWidget(widget);
}

ComponentDockWidget::~ComponentDockWidget()
{

}

void ComponentDockWidget::reflect(std::shared_ptr<Destiny::Reflection> reflection, QTableWidget* tableWidget)
{
	rttr::type type = rttr::type::get_by_name(reflection->get_class_name());
	auto properties = type.get_properties();
	size_t size = properties.size();

	m_tableWidget->clear();
	m_tableWidget->setColumnCount(2);
	m_tableWidget->setRowCount((int)size);
	m_tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("key"));
	m_tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("value"));

	int index = 0;
	for (auto& prop : type.get_properties())
	{
		m_tableWidget->setItem(index, 0, new QTableWidgetItem(prop.get_name().to_string().c_str()));
		if (prop.get_type().get_name() == "classstd::basic_string<char,structstd::char_traits<char>,classstd::allocator<char> >")

		{
			QLineEdit* lineEdit = new QLineEdit(m_tableWidget);
			lineEdit->setText(prop.get_value(reflection).to_string().c_str());
			m_tableWidget->setCellWidget(index, 1, lineEdit);
			connect(lineEdit, &QLineEdit::textChanged, this,
				[=]()
				{
					prop.set_value(reflection, lineEdit->text().toStdString());
				});
		}

		index++;
	}
}

void ComponentDockWidget::onChooseNode(std::shared_ptr<Destiny::Node3D> node)
{
	reflect(node, m_tableWidget);
}


