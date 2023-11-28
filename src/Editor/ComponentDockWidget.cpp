#include "ComponentDockWidget.h"
#include "Scene/Node3D.h"
#include "Scene/Component.h"
#include "Scene/Transform3D.h"
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

#define DRAW_FLOAT3(NAME, LABEL_NAME) \
	QLabel* label_##NAME = new QLabel(LABEL_NAME);\
	QHBoxLayout* hBoxLayout_##NAME = new QHBoxLayout();\
	QLabel* label_x_##NAME = new QLabel("x");\
	QLabel* label_y_##NAME = new QLabel("y");\
	QLabel* label_z_##NAME = new QLabel("z");\
	QDoubleSpinBox* doubleSpinBox_x_##NAME = new QDoubleSpinBox();\
	doubleSpinBox_x_##NAME->setMaximum(DBL_MAX);\
	doubleSpinBox_x_##NAME->setMinimum(-DBL_MAX);\
	doubleSpinBox_x_##NAME->setValue(transform3D.get_##NAME().x);\
	QDoubleSpinBox* doubleSpinBox_y_##NAME = new QDoubleSpinBox();\
	doubleSpinBox_y_##NAME->setMaximum(DBL_MAX);\
	doubleSpinBox_y_##NAME->setMinimum(-DBL_MAX);\
	doubleSpinBox_y_##NAME->setValue(transform3D.get_##NAME().y);\
	QDoubleSpinBox* doubleSpinBox_z_##NAME = new QDoubleSpinBox();\
	doubleSpinBox_z_##NAME->setMaximum(DBL_MAX);\
	doubleSpinBox_z_##NAME->setMinimum(-DBL_MAX);\
	doubleSpinBox_z_##NAME->setValue(transform3D.get_##NAME().z);\
	hBoxLayout_##NAME->addWidget(label_##NAME);\
	hBoxLayout_##NAME->addWidget(label_x_##NAME);\
	hBoxLayout_##NAME->addWidget(doubleSpinBox_x_##NAME);\
	hBoxLayout_##NAME->addWidget(label_y_##NAME);\
	hBoxLayout_##NAME->addWidget(doubleSpinBox_y_##NAME);\
	hBoxLayout_##NAME->addWidget(label_z_##NAME);\
	hBoxLayout_##NAME->addWidget(doubleSpinBox_z_##NAME);
	


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
	tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
		else if (prop.get_type().get_name() == "float")
		{
			QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox(tableWidget);
			doubleSpinBox->setMaximum(DBL_MAX);
			doubleSpinBox->setMinimum(-DBL_MAX);
			doubleSpinBox->setValue(prop.get_value(reflection).to_float());
			tableWidget->setCellWidget(index, 1, doubleSpinBox);
			connect(doubleSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, 
				[=]()
				{
					prop.set_value(reflection, (float)doubleSpinBox->value());
				});
		}
		else if (prop.get_type().get_name() == "XMFLOAT3")
		{
			using namespace DirectX;

			XMFLOAT3 float3 = prop.get_value(reflection).get_value<XMFLOAT3>();

			QHBoxLayout* hBoxLayout = new QHBoxLayout(tableWidget);
			QLabel* label_x = new QLabel("x", tableWidget);
			QLabel* label_y = new QLabel("y", tableWidget);
			QLabel* label_z = new QLabel("z", tableWidget);
			QDoubleSpinBox* doubleSpinBox_x = new QDoubleSpinBox(tableWidget);
			doubleSpinBox_x->setMaximum(DBL_MAX);
			doubleSpinBox_x->setMinimum(-DBL_MAX);
			doubleSpinBox_x->setValue(float3.x);
			QDoubleSpinBox* doubleSpinBox_y = new QDoubleSpinBox(tableWidget);
			doubleSpinBox_y->setMaximum(DBL_MAX);
			doubleSpinBox_y->setMinimum(-DBL_MAX);
			doubleSpinBox_y->setValue(float3.y);
			QDoubleSpinBox* doubleSpinBox_z = new QDoubleSpinBox(tableWidget);
			doubleSpinBox_z->setMaximum(DBL_MAX);
			doubleSpinBox_z->setMinimum(-DBL_MAX);
			doubleSpinBox_z->setValue(float3.z);
			hBoxLayout->addWidget(label_x);
			hBoxLayout->addWidget(doubleSpinBox_x);
			hBoxLayout->addWidget(label_y);
			hBoxLayout->addWidget(doubleSpinBox_y);
			hBoxLayout->addWidget(label_z);
			hBoxLayout->addWidget(doubleSpinBox_z);

			QWidget* widget = new QWidget(tableWidget);
			widget->setLayout(hBoxLayout);
			tableWidget->setCellWidget(index, 1, widget);

			auto func = 
			[=]()
			{
				prop.set_value(reflection, XMFLOAT3((float)doubleSpinBox_x->value(), (float)doubleSpinBox_y->value(), (float)doubleSpinBox_z->value()));
			};
			connect(doubleSpinBox_x, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
		}
		else if (prop.get_type().get_name() == "Transform3D")
		{
			using namespace DirectX;
			using namespace Destiny;

			Transform3D transform3D = prop.get_value(reflection).get_value<Transform3D>();
			DRAW_FLOAT3(translation, "translation:");
			DRAW_FLOAT3(rotation, "rotation:");
			DRAW_FLOAT3(scale, "scale:");


			QVBoxLayout* vBoxLayout = new QVBoxLayout();
			vBoxLayout->addLayout(hBoxLayout_translation);
			vBoxLayout->addLayout(hBoxLayout_rotation);
			vBoxLayout->addLayout(hBoxLayout_scale);
			QWidget* widget = new QWidget(tableWidget);
			widget->setLayout(vBoxLayout);
			tableWidget->setCellWidget(index, 1, widget);

			auto func =
				[=]()
				{
					Transform3D transform;
					transform.set_translation({ (float)doubleSpinBox_x_translation->value(), (float)doubleSpinBox_y_translation->value(), (float)doubleSpinBox_z_translation->value() });
					transform.set_rotation({ (float)doubleSpinBox_x_rotation->value(), (float)doubleSpinBox_y_rotation->value(), (float)doubleSpinBox_z_rotation->value() });
					transform.set_scale({ (float)doubleSpinBox_x_scale->value(), (float)doubleSpinBox_y_scale->value(), (float)doubleSpinBox_z_scale->value() });
					prop.set_value(reflection, transform);
				};
			connect(doubleSpinBox_x_translation, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_y_translation, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_z_translation, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);

			connect(doubleSpinBox_x_rotation, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_y_rotation, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_z_rotation, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);

			connect(doubleSpinBox_x_scale, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_y_scale, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_z_scale, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
		}
		else
		{
			auto name = prop.get_type().get_name();
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


