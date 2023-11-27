#include "ComponentDockWidget.h"
#include "Scene/Node3D.h"
#include "Scene/Component.h"
#include <DirectXMath.h>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <rttr/type>

ComponentDockWidget::ComponentDockWidget(QWidget *parent /*= nullptr*/) : QDockWidget("Property", parent)
{
	QWidget* widget = new QWidget();
	m_layout = new QVBoxLayout(widget);



	this->setWidget(widget);
}

ComponentDockWidget::~ComponentDockWidget()
{

}

void ComponentDockWidget::reflect(std::shared_ptr<Destiny::Reflection> reflection, QVBoxLayout* layout, bool clearLayout)
{
	if (!reflection || !layout)
	{
		return;
	}

	if (clearLayout)
	{
		QLayoutItem* child;
		while ((child = layout->takeAt(0)) != 0) {
			if (child->widget())
			{
				child->widget()->setParent(nullptr);
				delete child->widget();
			}
			delete child;
		}
	}

	QLabel* label = new QLabel();
	label->setText(reflection->get_class_name().c_str());

	rttr::type type = rttr::type::get_by_name(reflection->get_class_name());
	auto properties = type.get_properties();
	size_t size = properties.size();

	QTableWidget* tableWidget = new QTableWidget();
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	tableWidget->clear();
	tableWidget->setColumnCount(2);
	tableWidget->setRowCount((int)size);
	tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("key"));
	tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("value"));

	int index = 0;
	for (auto& prop : type.get_properties())
	{
		tableWidget->setItem(index, 0, new QTableWidgetItem(prop.get_name().to_string().c_str()));

		if (prop.get_type().get_name() == "classstd::basic_string<char,structstd::char_traits<char>,classstd::allocator<char> >")
		{
			QLineEdit* lineEdit = new QLineEdit(tableWidget);
			lineEdit->setText(prop.get_value(reflection).to_string().c_str());
			tableWidget->setCellWidget(index, 1, lineEdit);
			connect(lineEdit, &QLineEdit::textChanged, this,
				[=]()
				{
					prop.set_value(reflection, lineEdit->text().toStdString());
				});
		}
		else if (prop.get_type().get_name() == "bool")
		{
			QCheckBox* checkBox = new QCheckBox(tableWidget);
			checkBox->setChecked(prop.get_value(reflection).to_bool());
			tableWidget->setCellWidget(index, 1, checkBox);
			connect(checkBox, &QCheckBox::stateChanged, this,
				[=]()
				{
					prop.set_value(reflection, checkBox->isChecked());
				});
		}
		else if (prop.get_type().get_name() == "XMFLOAT3")
		{
			using namespace DirectX;

			QHBoxLayout* hBoxLayout = new QHBoxLayout(tableWidget);
			QLabel* label_x = new QLabel("x", tableWidget);
			QLabel* label_y = new QLabel("y", tableWidget);
			QLabel* label_z = new QLabel("z", tableWidget);
			QDoubleSpinBox* doubleSpinBox_x = new QDoubleSpinBox(tableWidget);
			doubleSpinBox_x->setMaximum(DBL_MAX);
			doubleSpinBox_x->setMinimum(-DBL_MAX);
			QDoubleSpinBox* doubleSpinBox_y = new QDoubleSpinBox(tableWidget);
			doubleSpinBox_y->setMaximum(DBL_MAX);
			doubleSpinBox_y->setMinimum(-DBL_MAX);
			QDoubleSpinBox* doubleSpinBox_z = new QDoubleSpinBox(tableWidget);
			doubleSpinBox_z->setMaximum(DBL_MAX);
			doubleSpinBox_z->setMinimum(-DBL_MAX);
			hBoxLayout->addWidget(label_x);
			hBoxLayout->addWidget(doubleSpinBox_x);
			hBoxLayout->addWidget(label_y);
			hBoxLayout->addWidget(doubleSpinBox_y);
			hBoxLayout->addWidget(label_z);
			hBoxLayout->addWidget(doubleSpinBox_z);

			QWidget* widget = new QWidget(tableWidget);
			widget->setLayout(hBoxLayout);
			tableWidget->setCellWidget(index, 1, widget);
		}
		else if (prop.get_type().get_name() == "Transform3D")
		{
			QGridLayout* gridLayout = new QGridLayout(tableWidget);
			QWidget* widget = new QWidget(tableWidget);
			QLabel* label_x = new QLabel("x", tableWidget);
			QLabel* label_y = new QLabel("y", tableWidget);
			QLabel* label_z = new QLabel("z", tableWidget);
			gridLayout->addWidget(label_x, 0, 0);
			gridLayout->addWidget(label_y, 0, 1);
			gridLayout->addWidget(label_z, 0, 2);
			widget->setLayout(gridLayout);
			tableWidget->setCellWidget(index, 1, widget);
		}
		else
		{
			tableWidget->setItem(index, 1, new QTableWidgetItem("Unsupported reflection type"));
		}

		index++;
	}

	layout->addWidget(label);
	layout->addWidget(tableWidget);	
}

void ComponentDockWidget::onChooseNode(std::shared_ptr<Destiny::Node3D> node)
{
	reflect(node, m_layout, true);
	for (auto& comp : node->get_components())
	{
		reflect(comp, m_layout, false);
	}
}


