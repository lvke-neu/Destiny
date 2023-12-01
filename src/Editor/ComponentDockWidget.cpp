#include "ComponentDockWidget.h"
#include "Scene/Node3D.h"
#include "Scene/Component.h"
#include "Scene/Transform3D.h"
#include "Graphics/Color.h"
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
#include <QComboBox>
#include <QColorDialog>
#include <d3d11.h>
#include <rttr/type>
#include <sstream>

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
	

Q_DECLARE_METATYPE(D3D11_FILL_MODE);
Q_DECLARE_METATYPE(D3D11_CULL_MODE);
Q_DECLARE_METATYPE(D3D11_DEPTH_WRITE_MASK);
Q_DECLARE_METATYPE(D3D11_COMPARISON_FUNC);

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
		else if (prop.get_type().get_name() == "D3D11_RASTERIZER_DESC")
		{
			QWidget* widget = new QWidget();
			QVBoxLayout* vBoxLayout = new QVBoxLayout();

			auto desc = prop.get_value(reflection).get_value<D3D11_RASTERIZER_DESC>();

			QComboBox* comboBox_FillMode = new QComboBox;
			comboBox_FillMode->addItem("D3D11_FILL_WIREFRAME", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
			comboBox_FillMode->addItem("D3D11_FILL_SOLID", D3D11_FILL_MODE::D3D11_FILL_SOLID);
			comboBox_FillMode->setCurrentIndex(desc.FillMode - 2);
			QHBoxLayout* hBoxLayout_FillMode = new QHBoxLayout;
			QLabel* label_FillMode = new QLabel("FillMode:");
			hBoxLayout_FillMode->addWidget(label_FillMode);
			hBoxLayout_FillMode->addWidget(comboBox_FillMode);

			
			QComboBox* comboBox_CullMode = new QComboBox;
			comboBox_CullMode->addItem("D3D11_CULL_NONE", D3D11_CULL_MODE::D3D11_CULL_NONE);
			comboBox_CullMode->addItem("D3D11_CULL_FRONT", D3D11_CULL_MODE::D3D11_CULL_FRONT);
			comboBox_CullMode->addItem("D3D11_CULL_BACK", D3D11_CULL_MODE::D3D11_CULL_BACK);
			comboBox_CullMode->setCurrentIndex(desc.FillMode - 1);
			QHBoxLayout* hBoxLayout_CullMode = new QHBoxLayout;
			QLabel* label_CullMode = new QLabel("CullMode:");
			hBoxLayout_CullMode->addWidget(label_CullMode);
			hBoxLayout_CullMode->addWidget(comboBox_CullMode);

			QCheckBox* checkBox_FrontCounterClockwise = new QCheckBox(tableWidget);
			checkBox_FrontCounterClockwise->setChecked(desc.FrontCounterClockwise);
			QHBoxLayout* hBoxLayout_FrontCounterClockwise = new QHBoxLayout();
			QLabel* label_FrontCounterClockwise = new QLabel("FrontCounterClockwise:");
			hBoxLayout_FrontCounterClockwise->addWidget(label_FrontCounterClockwise);
			hBoxLayout_FrontCounterClockwise->addWidget(checkBox_FrontCounterClockwise);
			

			QSpinBox* spinBox_DepthBias = new QSpinBox(tableWidget);
			spinBox_DepthBias->setMaximum(INT_MAX);
			spinBox_DepthBias->setMinimum(-INT_MAX);
			spinBox_DepthBias->setValue(desc.DepthBias);
			QHBoxLayout* hBoxLayout_DepthBias = new QHBoxLayout();
			QLabel* label_DepthBias = new QLabel("DepthBias:");
			hBoxLayout_DepthBias->addWidget(label_DepthBias);
			hBoxLayout_DepthBias->addWidget(spinBox_DepthBias);

			QDoubleSpinBox* doubleSpinBox_DepthBiasClamp = new QDoubleSpinBox;
			doubleSpinBox_DepthBiasClamp->setMaximum(DBL_MAX);
			doubleSpinBox_DepthBiasClamp->setMinimum(-DBL_MAX);
			doubleSpinBox_DepthBiasClamp->setValue(desc.DepthBiasClamp);
			QHBoxLayout* hBoxLayout_DepthBiasClamp = new QHBoxLayout();
			QLabel* label_DepthBiasClamp = new QLabel("DepthBiasClamp:");
			hBoxLayout_DepthBiasClamp->addWidget(label_DepthBiasClamp);
			hBoxLayout_DepthBiasClamp->addWidget(doubleSpinBox_DepthBiasClamp);

			QDoubleSpinBox* doubleSpinBox_SlopeScaledDepthBias = new QDoubleSpinBox;
			doubleSpinBox_SlopeScaledDepthBias->setMaximum(DBL_MAX);
			doubleSpinBox_SlopeScaledDepthBias->setMinimum(-DBL_MAX);
			doubleSpinBox_SlopeScaledDepthBias->setValue(desc.SlopeScaledDepthBias);
			QHBoxLayout* hBoxLayout_SlopeScaledDepthBias = new QHBoxLayout();
			QLabel* label_SlopeScaledDepthBias = new QLabel("SlopeScaledDepthBias:");
			hBoxLayout_SlopeScaledDepthBias->addWidget(label_SlopeScaledDepthBias);
			hBoxLayout_SlopeScaledDepthBias->addWidget(doubleSpinBox_SlopeScaledDepthBias);

			QCheckBox* checkBox_DepthClipEnable = new QCheckBox(tableWidget);
			checkBox_DepthClipEnable->setChecked(desc.DepthClipEnable);
			QHBoxLayout* hBoxLayout_DepthClipEnable = new QHBoxLayout();
			QLabel* label_DepthClipEnable = new QLabel("DepthClipEnable:");
			hBoxLayout_DepthClipEnable->addWidget(label_DepthClipEnable);
			hBoxLayout_DepthClipEnable->addWidget(checkBox_DepthClipEnable);
			

			QCheckBox* checkBox_ScissorEnable = new QCheckBox(tableWidget);
			checkBox_ScissorEnable->setChecked(desc.ScissorEnable);
			QHBoxLayout* hBoxLayout_ScissorEnable = new QHBoxLayout();
			QLabel* label_ScissorEnable = new QLabel("ScissorEnable:");
			hBoxLayout_ScissorEnable->addWidget(label_ScissorEnable);
			hBoxLayout_ScissorEnable->addWidget(checkBox_ScissorEnable);
			


			QCheckBox* checkBox_MultisampleEnable = new QCheckBox(tableWidget);
			checkBox_MultisampleEnable->setChecked(desc.MultisampleEnable);
			QHBoxLayout* hBoxLayout_MultisampleEnable = new QHBoxLayout();
			QLabel* label_MultisampleEnable = new QLabel("MultisampleEnable:");
			hBoxLayout_MultisampleEnable->addWidget(label_MultisampleEnable);
			hBoxLayout_MultisampleEnable->addWidget(checkBox_MultisampleEnable);
			

			QCheckBox* checkBox_AntialiasedLineEnable = new QCheckBox(tableWidget);
			checkBox_AntialiasedLineEnable->setChecked(desc.AntialiasedLineEnable);
			QHBoxLayout* hBoxLayout_AntialiasedLineEnable = new QHBoxLayout();
			QLabel* label_AntialiasedLineEnable = new QLabel("AntialiasedLineEnable:");
			hBoxLayout_AntialiasedLineEnable->addWidget(label_AntialiasedLineEnable);
			hBoxLayout_AntialiasedLineEnable->addWidget(checkBox_AntialiasedLineEnable);
			
			auto func =
				[=]()
			{
				D3D11_RASTERIZER_DESC tmpDesc;
				tmpDesc.FillMode = comboBox_FillMode->currentData().value<D3D11_FILL_MODE>();
				tmpDesc.CullMode = comboBox_CullMode->currentData().value<D3D11_CULL_MODE>();
				tmpDesc.FrontCounterClockwise = checkBox_FrontCounterClockwise->isChecked();
				tmpDesc.DepthBias = spinBox_DepthBias->value();
				tmpDesc.DepthBiasClamp = doubleSpinBox_DepthBiasClamp->value();
				tmpDesc.SlopeScaledDepthBias = doubleSpinBox_SlopeScaledDepthBias->value();
				tmpDesc.DepthClipEnable = checkBox_DepthClipEnable->isChecked();
				tmpDesc.ScissorEnable = checkBox_ScissorEnable->isChecked();
				tmpDesc.MultisampleEnable = checkBox_MultisampleEnable->isChecked();
				tmpDesc.AntialiasedLineEnable = checkBox_AntialiasedLineEnable->isChecked();
				prop.set_value(reflection, tmpDesc);
			}; 
			connect(comboBox_FillMode, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_CullMode, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(checkBox_FrontCounterClockwise, &QCheckBox::stateChanged, this, func);
			connect(spinBox_DepthBias, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_DepthBiasClamp, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(doubleSpinBox_SlopeScaledDepthBias, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, func);
			connect(checkBox_DepthClipEnable, &QCheckBox::stateChanged, this, func);
			connect(checkBox_ScissorEnable, &QCheckBox::stateChanged, this, func);
			connect(checkBox_MultisampleEnable, &QCheckBox::stateChanged, this, func);
			connect(checkBox_AntialiasedLineEnable, &QCheckBox::stateChanged, this, func);
			vBoxLayout->addLayout(hBoxLayout_FillMode);
			vBoxLayout->addLayout(hBoxLayout_CullMode);
			vBoxLayout->addLayout(hBoxLayout_FrontCounterClockwise);
			vBoxLayout->addLayout(hBoxLayout_DepthBias);
			vBoxLayout->addLayout(hBoxLayout_DepthBiasClamp);
			vBoxLayout->addLayout(hBoxLayout_SlopeScaledDepthBias);
			vBoxLayout->addLayout(hBoxLayout_DepthClipEnable);
			vBoxLayout->addLayout(hBoxLayout_ScissorEnable);
			vBoxLayout->addLayout(hBoxLayout_MultisampleEnable);
			vBoxLayout->addLayout(hBoxLayout_AntialiasedLineEnable);
			widget->setLayout(vBoxLayout);
			tableWidget->setCellWidget(index, 1, widget);
		}
		else if (prop.get_type().get_name() == "D3D11_DEPTH_STENCIL_DESC")
		{
			QWidget* widget = new QWidget();
			QVBoxLayout* vBoxLayout = new QVBoxLayout();

			auto desc = prop.get_value(reflection).get_value<D3D11_DEPTH_STENCIL_DESC>();

			QCheckBox* checkBox_DepthEnable = new QCheckBox();
			checkBox_DepthEnable->setChecked(desc.DepthEnable);
			QHBoxLayout* hBoxLayout_DepthEnable = new QHBoxLayout();
			QLabel* label_DepthEnable = new QLabel("DepthEnable:");
			hBoxLayout_DepthEnable->addWidget(label_DepthEnable);
			hBoxLayout_DepthEnable->addWidget(checkBox_DepthEnable);

			QComboBox* comboBox_DepthWriteMask = new QComboBox;
			comboBox_DepthWriteMask->addItem("D3D11_DEPTH_WRITE_MASK_ZERO", D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO);
			comboBox_DepthWriteMask->addItem("D3D11_DEPTH_WRITE_MASK_ALL", D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL);
			comboBox_DepthWriteMask->setCurrentIndex(desc.DepthWriteMask);
			QHBoxLayout* hBoxLayout_DepthWriteMask = new QHBoxLayout;
			QLabel* label_DepthWriteMask = new QLabel("DepthWriteMask:");
			hBoxLayout_DepthWriteMask->addWidget(label_DepthWriteMask);
			hBoxLayout_DepthWriteMask->addWidget(comboBox_DepthWriteMask);

			QComboBox* comboBox_DepthFunc = new QComboBox;
			comboBox_DepthFunc->addItem("D3D11_DEPTH_WRITE_MASK_ZERO", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER);
			comboBox_DepthFunc->addItem("D3D11_COMPARISON_LESS", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS);
			comboBox_DepthFunc->addItem("D3D11_COMPARISON_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL);
			comboBox_DepthFunc->addItem("D3D11_COMPARISON_LESS_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL);
			comboBox_DepthFunc->addItem("D3D11_COMPARISON_GREATER", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER);
			comboBox_DepthFunc->addItem("D3D11_COMPARISON_NOT_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL);
			comboBox_DepthFunc->addItem("D3D11_COMPARISON_GREATER_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER_EQUAL);
			comboBox_DepthFunc->addItem("D3D11_COMPARISON_ALWAYS", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS);
			comboBox_DepthFunc->setCurrentIndex(desc.DepthFunc - 1);
			QHBoxLayout* hBoxLayout_DepthFunc = new QHBoxLayout;
			QLabel* label_DepthFunc = new QLabel("DepthFunc:");
			hBoxLayout_DepthFunc->addWidget(label_DepthFunc);
			hBoxLayout_DepthFunc->addWidget(comboBox_DepthFunc);

			QCheckBox* checkBox_StencilEnable = new QCheckBox();
			checkBox_StencilEnable->setChecked(desc.StencilEnable);
			QHBoxLayout* hBoxLayout_StencilEnable = new QHBoxLayout();
			QLabel* label_StencilEnable = new QLabel("StencilEnable:");
			hBoxLayout_StencilEnable->addWidget(label_StencilEnable);
			hBoxLayout_StencilEnable->addWidget(checkBox_StencilEnable);

			auto func =
				[=]()
			{
				D3D11_DEPTH_STENCIL_DESC tmpDesc;
				tmpDesc.DepthEnable = checkBox_DepthEnable->isChecked();
				tmpDesc.DepthWriteMask = comboBox_DepthWriteMask->currentData().value<D3D11_DEPTH_WRITE_MASK>();
				tmpDesc.DepthFunc = comboBox_DepthFunc->currentData().value<D3D11_COMPARISON_FUNC>();
				tmpDesc.StencilEnable = checkBox_StencilEnable->isChecked();
				prop.set_value(reflection, tmpDesc);
			};
			connect(checkBox_DepthEnable, &QCheckBox::stateChanged, this, func);
			connect(comboBox_DepthWriteMask, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_DepthFunc, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(checkBox_StencilEnable, &QCheckBox::stateChanged, this, func);
			vBoxLayout->addLayout(hBoxLayout_DepthEnable);
			vBoxLayout->addLayout(hBoxLayout_DepthWriteMask);
			vBoxLayout->addLayout(hBoxLayout_DepthFunc);
			vBoxLayout->addLayout(hBoxLayout_StencilEnable);

			widget->setLayout(vBoxLayout);
			tableWidget->setCellWidget(index, 1, widget);
		}
		else if (prop.get_type().get_name() == "Color")
		{
			using namespace Destiny;
			auto color = prop.get_value(reflection).get_value<Color>();
			std::stringstream str;
			str << "QPushButton{background-color:rgba(";
			str << (int)(color.get_r() * 255) << ",";
			str << (int)(color.get_g() * 255) << ",";
			str << (int)(color.get_b() * 255) << ",";
			str << (int)(color.get_a() * 255) << ");}";

			std::string styleSheet;
			str >> styleSheet;

			QPushButton* pushButton = new QPushButton(tableWidget);
			pushButton->setStyleSheet(styleSheet.c_str());
			tableWidget->setCellWidget(index, 1, pushButton);

			connect(pushButton, &QPushButton::clicked, this,
			[=]()
			{			
				QColorDialog dlg(tableWidget);

				dlg.setWindowTitle("Choose Color");
				dlg.setCurrentColor(QColor((int)(color.get_r() * 255), (int)(color.get_g() * 255), (int)(color.get_b() * 255), (int)(color.get_a() * 255)));
				connect(&dlg, &QColorDialog::currentColorChanged, this, [&](const QColor& qColor)
					{
						Color tmpColor(qColor.red() / 255.0f, qColor.green() / 255.0f, qColor.blue() / 255.0f, qColor.alpha() / 255.0f);
						prop.set_value(reflection, tmpColor);
						
						std::stringstream strTmp;
						strTmp << "QPushButton{background-color:rgba(";
						strTmp << (int)(tmpColor.get_r() * 255) << ",";
						strTmp << (int)(tmpColor.get_g() * 255) << ",";
						strTmp << (int)(tmpColor.get_b() * 255) << ",";
						strTmp << (int)(tmpColor.get_a() * 255) << ");}";

						std::string styleSheetTmp;
						strTmp >> styleSheetTmp;
						pushButton->setStyleSheet(styleSheetTmp.c_str());
					});

				if (dlg.exec() != QColorDialog::Accepted)
				{
					prop.set_value(reflection, color);
					pushButton->setStyleSheet(styleSheet.c_str());
				}
				
			});
		}
		else
		{
			auto name = prop.get_type().get_name();
			tableWidget->setItem(index, 1, new QTableWidgetItem(("Unsupport Type : " + name.to_string()).c_str()));
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


