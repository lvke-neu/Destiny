#include "ComponentDockWidget.h"
#include "Scene/Node3D.h"
#include "Scene/Component.h"
#include "Scene/Transform3D.h"
#include "Graphics/Color.h"
#include "Graphics/Material.h"
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
Q_DECLARE_METATYPE(D3D11_STENCIL_OP);
Q_DECLARE_METATYPE(D3D11_BLEND);
Q_DECLARE_METATYPE(D3D11_BLEND_OP);

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

			QSpinBox* spinBox_StencilReadMask = new QSpinBox(tableWidget);
			spinBox_StencilReadMask->setMaximum(255);
			spinBox_StencilReadMask->setMinimum(0);
			spinBox_StencilReadMask->setValue(desc.StencilReadMask);
			QHBoxLayout* hBoxLayout_StencilReadMask = new QHBoxLayout();
			QLabel* label_StencilReadMask = new QLabel("StencilReadMask:");
			hBoxLayout_StencilReadMask->addWidget(label_StencilReadMask);
			hBoxLayout_StencilReadMask->addWidget(spinBox_StencilReadMask);

			QSpinBox* spinBox_StencilWriteMask = new QSpinBox(tableWidget);
			spinBox_StencilWriteMask->setMaximum(255);
			spinBox_StencilWriteMask->setMinimum(0);
			spinBox_StencilWriteMask->setValue(desc.StencilWriteMask);
			QHBoxLayout* hBoxLayout_StencilWriteMask = new QHBoxLayout();
			QLabel* label_StencilWriteMask = new QLabel("StencilWriteMask:");
			hBoxLayout_StencilWriteMask->addWidget(label_StencilWriteMask);
			hBoxLayout_StencilWriteMask->addWidget(spinBox_StencilWriteMask);

			
			//front
			QComboBox* comboBox_FrontStencilFailOp = new QComboBox;
			comboBox_FrontStencilFailOp->addItem("D3D11_STENCIL_OP_KEEP", D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP);
			comboBox_FrontStencilFailOp->addItem("D3D11_STENCIL_OP_ZERO", D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO);
			comboBox_FrontStencilFailOp->addItem("D3D11_STENCIL_OP_REPLACE", D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE);
			comboBox_FrontStencilFailOp->addItem("D3D11_STENCIL_OP_INCR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT);
			comboBox_FrontStencilFailOp->addItem("D3D11_STENCIL_OP_DECR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT);
			comboBox_FrontStencilFailOp->addItem("D3D11_STENCIL_OP_INVERT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT);
			comboBox_FrontStencilFailOp->addItem("D3D11_STENCIL_OP_INCR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR);
			comboBox_FrontStencilFailOp->addItem("D3D11_STENCIL_OP_DECR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR);
			comboBox_FrontStencilFailOp->setCurrentIndex(desc.FrontFace.StencilFailOp - 1);
			QHBoxLayout* hBoxLayout_FrontStencilFailOp = new QHBoxLayout;
			QLabel* label_FrontStencilFailOp = new QLabel("FrontStencilFailOp:");
			hBoxLayout_FrontStencilFailOp->addWidget(label_FrontStencilFailOp);
			hBoxLayout_FrontStencilFailOp->addWidget(comboBox_FrontStencilFailOp);

			QComboBox* comboBox_FrontStencilDepthFailOp = new QComboBox;
			comboBox_FrontStencilDepthFailOp->addItem("D3D11_STENCIL_OP_KEEP", D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP);
			comboBox_FrontStencilDepthFailOp->addItem("D3D11_STENCIL_OP_ZERO", D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO);
			comboBox_FrontStencilDepthFailOp->addItem("D3D11_STENCIL_OP_REPLACE", D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE);
			comboBox_FrontStencilDepthFailOp->addItem("D3D11_STENCIL_OP_INCR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT);
			comboBox_FrontStencilDepthFailOp->addItem("D3D11_STENCIL_OP_DECR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT);
			comboBox_FrontStencilDepthFailOp->addItem("D3D11_STENCIL_OP_INVERT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT);
			comboBox_FrontStencilDepthFailOp->addItem("D3D11_STENCIL_OP_INCR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR);
			comboBox_FrontStencilDepthFailOp->addItem("D3D11_STENCIL_OP_DECR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR);
			comboBox_FrontStencilDepthFailOp->setCurrentIndex(desc.FrontFace.StencilDepthFailOp - 1);
			QHBoxLayout* hBoxLayout_FrontStencilDepthFailOp = new QHBoxLayout;
			QLabel* label_FrontStencilDepthFailOp = new QLabel("FrontStencilDepthFailOp:");
			hBoxLayout_FrontStencilDepthFailOp->addWidget(label_FrontStencilDepthFailOp);
			hBoxLayout_FrontStencilDepthFailOp->addWidget(comboBox_FrontStencilDepthFailOp);

			QComboBox* comboBox_FrontStencilPassOp = new QComboBox;
			comboBox_FrontStencilPassOp->addItem("D3D11_STENCIL_OP_KEEP", D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP);
			comboBox_FrontStencilPassOp->addItem("D3D11_STENCIL_OP_ZERO", D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO);
			comboBox_FrontStencilPassOp->addItem("D3D11_STENCIL_OP_REPLACE", D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE);
			comboBox_FrontStencilPassOp->addItem("D3D11_STENCIL_OP_INCR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT);
			comboBox_FrontStencilPassOp->addItem("D3D11_STENCIL_OP_DECR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT);
			comboBox_FrontStencilPassOp->addItem("D3D11_STENCIL_OP_INVERT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT);
			comboBox_FrontStencilPassOp->addItem("D3D11_STENCIL_OP_INCR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR);
			comboBox_FrontStencilPassOp->addItem("D3D11_STENCIL_OP_DECR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR);
			comboBox_FrontStencilPassOp->setCurrentIndex(desc.FrontFace.StencilPassOp - 1);
			QHBoxLayout* hBoxLayout_FrontStencilPassOp = new QHBoxLayout;
			QLabel* label_FrontStencilPassOp = new QLabel("FrontStencilPassOp:");
			hBoxLayout_FrontStencilPassOp->addWidget(label_FrontStencilPassOp);
			hBoxLayout_FrontStencilPassOp->addWidget(comboBox_FrontStencilPassOp);


			QComboBox* comboBox_FrontStencilFunc = new QComboBox;
			comboBox_FrontStencilFunc->addItem("D3D11_COMPARISON_NEVER", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER);
			comboBox_FrontStencilFunc->addItem("D3D11_COMPARISON_LESS", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS);
			comboBox_FrontStencilFunc->addItem("D3D11_COMPARISON_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL);
			comboBox_FrontStencilFunc->addItem("D3D11_COMPARISON_LESS_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL);
			comboBox_FrontStencilFunc->addItem("D3D11_COMPARISON_GREATER", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER);
			comboBox_FrontStencilFunc->addItem("D3D11_COMPARISON_NOT_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL);
			comboBox_FrontStencilFunc->addItem("D3D11_COMPARISON_GREATER_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER_EQUAL);
			comboBox_FrontStencilFunc->addItem("D3D11_COMPARISON_ALWAYS", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS);
			comboBox_FrontStencilFunc->setCurrentIndex(desc.FrontFace.StencilFunc - 1);
			QHBoxLayout* hBoxLayout_FrontStencilFunc = new QHBoxLayout;
			QLabel* label_FrontStencilFunc = new QLabel("FrontStencilFunc:");
			hBoxLayout_FrontStencilFunc->addWidget(label_FrontStencilFunc);
			hBoxLayout_FrontStencilFunc->addWidget(comboBox_FrontStencilFunc);


			//back
			QComboBox* comboBox_BackStencilFailOp = new QComboBox;
			comboBox_BackStencilFailOp->addItem("D3D11_STENCIL_OP_KEEP", D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP);
			comboBox_BackStencilFailOp->addItem("D3D11_STENCIL_OP_ZERO", D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO);
			comboBox_BackStencilFailOp->addItem("D3D11_STENCIL_OP_REPLACE", D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE);
			comboBox_BackStencilFailOp->addItem("D3D11_STENCIL_OP_INCR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT);
			comboBox_BackStencilFailOp->addItem("D3D11_STENCIL_OP_DECR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT);
			comboBox_BackStencilFailOp->addItem("D3D11_STENCIL_OP_INVERT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT);
			comboBox_BackStencilFailOp->addItem("D3D11_STENCIL_OP_INCR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR);
			comboBox_BackStencilFailOp->addItem("D3D11_STENCIL_OP_DECR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR);
			comboBox_BackStencilFailOp->setCurrentIndex(desc.BackFace.StencilFailOp - 1);
			QHBoxLayout* hBoxLayout_BackStencilFailOp = new QHBoxLayout;
			QLabel* label_BackStencilFailOp = new QLabel("BackStencilFailOp:");
			hBoxLayout_BackStencilFailOp->addWidget(label_BackStencilFailOp);
			hBoxLayout_BackStencilFailOp->addWidget(comboBox_BackStencilFailOp);

			QComboBox* comboBox_BackStencilDepthFailOp = new QComboBox;
			comboBox_BackStencilDepthFailOp->addItem("D3D11_STENCIL_OP_KEEP", D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP);
			comboBox_BackStencilDepthFailOp->addItem("D3D11_STENCIL_OP_ZERO", D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO);
			comboBox_BackStencilDepthFailOp->addItem("D3D11_STENCIL_OP_REPLACE", D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE);
			comboBox_BackStencilDepthFailOp->addItem("D3D11_STENCIL_OP_INCR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT);
			comboBox_BackStencilDepthFailOp->addItem("D3D11_STENCIL_OP_DECR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT);
			comboBox_BackStencilDepthFailOp->addItem("D3D11_STENCIL_OP_INVERT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT);
			comboBox_BackStencilDepthFailOp->addItem("D3D11_STENCIL_OP_INCR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR);
			comboBox_BackStencilDepthFailOp->addItem("D3D11_STENCIL_OP_DECR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR);
			comboBox_BackStencilDepthFailOp->setCurrentIndex(desc.BackFace.StencilDepthFailOp - 1);
			QHBoxLayout* hBoxLayout_BackStencilDepthFailOp = new QHBoxLayout;
			QLabel* label_BackStencilDepthFailOp = new QLabel("BackStencilDepthFailOp:");
			hBoxLayout_BackStencilDepthFailOp->addWidget(label_BackStencilDepthFailOp);
			hBoxLayout_BackStencilDepthFailOp->addWidget(comboBox_BackStencilDepthFailOp);

			QComboBox* comboBox_BackStencilPassOp = new QComboBox;
			comboBox_BackStencilPassOp->addItem("D3D11_STENCIL_OP_KEEP", D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP);
			comboBox_BackStencilPassOp->addItem("D3D11_STENCIL_OP_ZERO", D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO);
			comboBox_BackStencilPassOp->addItem("D3D11_STENCIL_OP_REPLACE", D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE);
			comboBox_BackStencilPassOp->addItem("D3D11_STENCIL_OP_INCR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT);
			comboBox_BackStencilPassOp->addItem("D3D11_STENCIL_OP_DECR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT);
			comboBox_BackStencilPassOp->addItem("D3D11_STENCIL_OP_INVERT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT);
			comboBox_BackStencilPassOp->addItem("D3D11_STENCIL_OP_INCR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR);
			comboBox_BackStencilPassOp->addItem("D3D11_STENCIL_OP_DECR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR);
			comboBox_BackStencilPassOp->setCurrentIndex(desc.BackFace.StencilPassOp - 1);
			QHBoxLayout* hBoxLayout_BackStencilPassOp = new QHBoxLayout;
			QLabel* label_BackStencilPassOp = new QLabel("BackStencilPassOp:");
			hBoxLayout_BackStencilPassOp->addWidget(label_BackStencilPassOp);
			hBoxLayout_BackStencilPassOp->addWidget(comboBox_BackStencilPassOp);


			QComboBox* comboBox_BackStencilFunc = new QComboBox;
			comboBox_BackStencilFunc->addItem("D3D11_COMPARISON_NEVER", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER);
			comboBox_BackStencilFunc->addItem("D3D11_COMPARISON_LESS", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS);
			comboBox_BackStencilFunc->addItem("D3D11_COMPARISON_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL);
			comboBox_BackStencilFunc->addItem("D3D11_COMPARISON_LESS_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL);
			comboBox_BackStencilFunc->addItem("D3D11_COMPARISON_GREATER", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER);
			comboBox_BackStencilFunc->addItem("D3D11_COMPARISON_NOT_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL);
			comboBox_BackStencilFunc->addItem("D3D11_COMPARISON_GREATER_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER_EQUAL);
			comboBox_BackStencilFunc->addItem("D3D11_COMPARISON_ALWAYS", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS);
			comboBox_BackStencilFunc->setCurrentIndex(desc.BackFace.StencilFunc - 1);
			QHBoxLayout* hBoxLayout_BackStencilFunc = new QHBoxLayout;
			QLabel* label_BackStencilFunc = new QLabel("BackStencilFunc:");
			hBoxLayout_BackStencilFunc->addWidget(label_BackStencilFunc);
			hBoxLayout_BackStencilFunc->addWidget(comboBox_BackStencilFunc);

			auto func =
				[=]()
			{
				D3D11_DEPTH_STENCIL_DESC tmpDesc;
				tmpDesc.DepthEnable = checkBox_DepthEnable->isChecked();
				tmpDesc.DepthWriteMask = comboBox_DepthWriteMask->currentData().value<D3D11_DEPTH_WRITE_MASK>();
				tmpDesc.DepthFunc = comboBox_DepthFunc->currentData().value<D3D11_COMPARISON_FUNC>();
				tmpDesc.StencilEnable = checkBox_StencilEnable->isChecked();
				tmpDesc.StencilReadMask = spinBox_StencilReadMask->value();
				tmpDesc.StencilWriteMask = spinBox_StencilWriteMask->value();
				tmpDesc.FrontFace.StencilFailOp = comboBox_FrontStencilFailOp->currentData().value<D3D11_STENCIL_OP>();
				tmpDesc.FrontFace.StencilDepthFailOp = comboBox_FrontStencilDepthFailOp->currentData().value<D3D11_STENCIL_OP>();
				tmpDesc.FrontFace.StencilPassOp = comboBox_FrontStencilPassOp->currentData().value<D3D11_STENCIL_OP>();
				tmpDesc.FrontFace.StencilFunc = comboBox_FrontStencilFunc->currentData().value<D3D11_COMPARISON_FUNC>();
				tmpDesc.BackFace.StencilFailOp = comboBox_BackStencilFailOp->currentData().value<D3D11_STENCIL_OP>();
				tmpDesc.BackFace.StencilDepthFailOp = comboBox_BackStencilDepthFailOp->currentData().value<D3D11_STENCIL_OP>();
				tmpDesc.BackFace.StencilPassOp = comboBox_BackStencilPassOp->currentData().value<D3D11_STENCIL_OP>();
				tmpDesc.BackFace.StencilFunc = comboBox_BackStencilFunc->currentData().value<D3D11_COMPARISON_FUNC>();
				prop.set_value(reflection, tmpDesc);
			};
			connect(checkBox_DepthEnable, &QCheckBox::stateChanged, this, func);
			connect(comboBox_DepthWriteMask, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_DepthFunc, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(checkBox_StencilEnable, &QCheckBox::stateChanged, this, func);
			connect(spinBox_StencilReadMask, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, func);
			connect(spinBox_StencilWriteMask, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, func);
			connect(comboBox_FrontStencilFailOp, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_FrontStencilDepthFailOp, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_FrontStencilPassOp, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_FrontStencilFunc, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_BackStencilFailOp, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_BackStencilDepthFailOp, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_BackStencilPassOp, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_BackStencilFunc, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			vBoxLayout->addLayout(hBoxLayout_DepthEnable);
			vBoxLayout->addLayout(hBoxLayout_DepthWriteMask);
			vBoxLayout->addLayout(hBoxLayout_DepthFunc);
			vBoxLayout->addLayout(hBoxLayout_StencilEnable);
			vBoxLayout->addLayout(hBoxLayout_StencilReadMask);
			vBoxLayout->addLayout(hBoxLayout_StencilWriteMask);
			vBoxLayout->addLayout(hBoxLayout_FrontStencilFailOp);
			vBoxLayout->addLayout(hBoxLayout_FrontStencilDepthFailOp);
			vBoxLayout->addLayout(hBoxLayout_FrontStencilPassOp);
			vBoxLayout->addLayout(hBoxLayout_FrontStencilFunc);
			vBoxLayout->addLayout(hBoxLayout_BackStencilFailOp);
			vBoxLayout->addLayout(hBoxLayout_BackStencilDepthFailOp);
			vBoxLayout->addLayout(hBoxLayout_BackStencilPassOp);
			vBoxLayout->addLayout(hBoxLayout_BackStencilFunc);

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
		else if (prop.get_type().get_name() == "D3D11_BLEND_DESC")
		{
			QWidget* widget = new QWidget();
			QVBoxLayout* vBoxLayout = new QVBoxLayout();

			auto desc = prop.get_value(reflection).get_value<D3D11_BLEND_DESC>();

			QCheckBox* checkBox_AlphaToCoverageEnable = new QCheckBox();
			checkBox_AlphaToCoverageEnable->setChecked(desc.AlphaToCoverageEnable);
			QHBoxLayout* hBoxLayout_AlphaToCoverageEnable = new QHBoxLayout();
			QLabel* label_AlphaToCoverageEnable = new QLabel("AlphaToCoverageEnable:");
			hBoxLayout_AlphaToCoverageEnable->addWidget(label_AlphaToCoverageEnable);
			hBoxLayout_AlphaToCoverageEnable->addWidget(checkBox_AlphaToCoverageEnable);

			QCheckBox* checkBox_IndependentBlendEnable = new QCheckBox();
			checkBox_IndependentBlendEnable->setChecked(desc.IndependentBlendEnable);
			QHBoxLayout* hBoxLayout_IndependentBlendEnable = new QHBoxLayout();
			QLabel* label_IndependentBlendEnable = new QLabel("IndependentBlendEnable:");
			hBoxLayout_IndependentBlendEnable->addWidget(label_IndependentBlendEnable);
			hBoxLayout_IndependentBlendEnable->addWidget(checkBox_IndependentBlendEnable);

			QCheckBox* checkBox_BlendEnable = new QCheckBox();
			checkBox_BlendEnable->setChecked(desc.RenderTarget[0].BlendEnable);
			QHBoxLayout* hBoxLayout_BlendEnable = new QHBoxLayout();
			QLabel* label_BlendEnable = new QLabel("BlendEnable:");
			hBoxLayout_BlendEnable->addWidget(label_BlendEnable);
			hBoxLayout_BlendEnable->addWidget(checkBox_BlendEnable);

			
			QComboBox* comboBox_SrcBlend = new QComboBox;
			comboBox_SrcBlend->addItem("D3D11_BLEND_ZERO", D3D11_BLEND::D3D11_BLEND_ZERO);
			comboBox_SrcBlend->addItem("D3D11_BLEND_ONE", D3D11_BLEND::D3D11_BLEND_ONE);
			comboBox_SrcBlend->addItem("D3D11_BLEND_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_SRC_COLOR);
			comboBox_SrcBlend->addItem("D3D11_BLEND_INV_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR);
			comboBox_SrcBlend->addItem("D3D11_BLEND_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA);
			comboBox_SrcBlend->addItem("D3D11_BLEND_INV_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA);
			comboBox_SrcBlend->addItem("D3D11_BLEND_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_DEST_ALPHA);
			comboBox_SrcBlend->addItem("D3D11_BLEND_INV_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA);
			comboBox_SrcBlend->addItem("D3D11_BLEND_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_DEST_COLOR);
			comboBox_SrcBlend->addItem("D3D11_BLEND_INV_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR);
			comboBox_SrcBlend->addItem("D3D11_BLEND_SRC_ALPHA_SAT", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA_SAT);
			comboBox_SrcBlend->addItem("D3D11_BLEND_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR);
			comboBox_SrcBlend->addItem("D3D11_BLEND_INV_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_INV_BLEND_FACTOR);
			comboBox_SrcBlend->addItem("D3D11_BLEND_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_SRC1_COLOR);
			comboBox_SrcBlend->addItem("D3D11_BLEND_INV_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC1_COLOR);
			comboBox_SrcBlend->addItem("D3D11_BLEND_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC1_ALPHA);
			comboBox_SrcBlend->addItem("D3D11_BLEND_INV_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC1_ALPHA);
			if (desc.RenderTarget[0].SrcBlend <= 11)
			{
				comboBox_SrcBlend->setCurrentIndex(desc.RenderTarget[0].SrcBlend - 1);
			}
			else
			{
				comboBox_SrcBlend->setCurrentIndex(desc.RenderTarget[0].SrcBlend - 3);
			}
			QHBoxLayout* hBoxLayout_SrcBlend = new QHBoxLayout;
			QLabel* label_SrcBlend = new QLabel("SrcBlend:");
			hBoxLayout_SrcBlend->addWidget(label_SrcBlend);
			hBoxLayout_SrcBlend->addWidget(comboBox_SrcBlend);
		

			QComboBox* comboBox_DestBlend = new QComboBox;
			comboBox_DestBlend->addItem("D3D11_BLEND_ZERO", D3D11_BLEND::D3D11_BLEND_ZERO);
			comboBox_DestBlend->addItem("D3D11_BLEND_ONE", D3D11_BLEND::D3D11_BLEND_ONE);
			comboBox_DestBlend->addItem("D3D11_BLEND_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_SRC_COLOR);
			comboBox_DestBlend->addItem("D3D11_BLEND_INV_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR);
			comboBox_DestBlend->addItem("D3D11_BLEND_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA);
			comboBox_DestBlend->addItem("D3D11_BLEND_INV_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA);
			comboBox_DestBlend->addItem("D3D11_BLEND_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_DEST_ALPHA);
			comboBox_DestBlend->addItem("D3D11_BLEND_INV_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA);
			comboBox_DestBlend->addItem("D3D11_BLEND_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_DEST_COLOR);
			comboBox_DestBlend->addItem("D3D11_BLEND_INV_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR);
			comboBox_DestBlend->addItem("D3D11_BLEND_SRC_ALPHA_SAT", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA_SAT);
			comboBox_DestBlend->addItem("D3D11_BLEND_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR);
			comboBox_DestBlend->addItem("D3D11_BLEND_INV_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_INV_BLEND_FACTOR);
			comboBox_DestBlend->addItem("D3D11_BLEND_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_SRC1_COLOR);
			comboBox_DestBlend->addItem("D3D11_BLEND_INV_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC1_COLOR);
			comboBox_DestBlend->addItem("D3D11_BLEND_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC1_ALPHA);
			comboBox_DestBlend->addItem("D3D11_BLEND_INV_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC1_ALPHA);
			if (desc.RenderTarget[0].DestBlend <= 11)
			{
				comboBox_SrcBlend->setCurrentIndex(desc.RenderTarget[0].DestBlend - 1);
			}
			else
			{
				comboBox_SrcBlend->setCurrentIndex(desc.RenderTarget[0].DestBlend - 3);
			}
			QHBoxLayout* hBoxLayout_DestBlend = new QHBoxLayout;
			QLabel* label_DestBlend = new QLabel("DestBlend:");
			hBoxLayout_DestBlend->addWidget(label_DestBlend);
			hBoxLayout_DestBlend->addWidget(comboBox_DestBlend);


			QComboBox* comboBox_BlendOp = new QComboBox;
			comboBox_BlendOp->addItem("D3D11_BLEND_OP_ADD", D3D11_BLEND_OP::D3D11_BLEND_OP_ADD);
			comboBox_BlendOp->addItem("D3D11_BLEND_OP_SUBTRACT", D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT);
			comboBox_BlendOp->addItem("D3D11_BLEND_OP_REV_SUBTRACT", D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT);
			comboBox_BlendOp->addItem("D3D11_BLEND_OP_MIN", D3D11_BLEND_OP::D3D11_BLEND_OP_MIN);
			comboBox_BlendOp->addItem("D3D11_BLEND_OP_MAX", D3D11_BLEND_OP::D3D11_BLEND_OP_MAX);
			comboBox_BlendOp->setCurrentIndex(desc.RenderTarget[0].BlendOp - 1);
			QHBoxLayout* hBoxLayout_BlendOp = new QHBoxLayout;
			QLabel* label_BlendOp = new QLabel("BlendOp:");
			hBoxLayout_BlendOp->addWidget(label_BlendOp);
			hBoxLayout_BlendOp->addWidget(comboBox_BlendOp);


			QComboBox* comboBox_SrcBlendAlpha = new QComboBox;
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_ZERO", D3D11_BLEND::D3D11_BLEND_ZERO);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_ONE", D3D11_BLEND::D3D11_BLEND_ONE);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_SRC_COLOR);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_INV_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_INV_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_DEST_ALPHA);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_INV_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_DEST_COLOR);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_INV_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_SRC_ALPHA_SAT", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA_SAT);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_INV_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_INV_BLEND_FACTOR);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_SRC1_COLOR);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_INV_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC1_COLOR);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC1_ALPHA);
			comboBox_SrcBlendAlpha->addItem("D3D11_BLEND_INV_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC1_ALPHA);
			if (desc.RenderTarget[0].SrcBlendAlpha <= 11)
			{
				comboBox_SrcBlendAlpha->setCurrentIndex(desc.RenderTarget[0].SrcBlendAlpha - 1);
			}
			else
			{
				comboBox_SrcBlendAlpha->setCurrentIndex(desc.RenderTarget[0].SrcBlendAlpha - 3);
			}
			QHBoxLayout* hBoxLayout_SrcBlendAlpha = new QHBoxLayout;
			QLabel* label_SrcBlendAlpha = new QLabel("SrcBlendAlpha:");
			hBoxLayout_SrcBlendAlpha->addWidget(label_SrcBlendAlpha);
			hBoxLayout_SrcBlendAlpha->addWidget(comboBox_SrcBlendAlpha);

			QComboBox* comboBox_DestBlendAlpha = new QComboBox;
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_ZERO", D3D11_BLEND::D3D11_BLEND_ZERO);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_ONE", D3D11_BLEND::D3D11_BLEND_ONE);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_SRC_COLOR);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_INV_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_INV_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_DEST_ALPHA);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_INV_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_DEST_COLOR);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_INV_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_SRC_ALPHA_SAT", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA_SAT);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_INV_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_INV_BLEND_FACTOR);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_SRC1_COLOR);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_INV_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC1_COLOR);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC1_ALPHA);
			comboBox_DestBlendAlpha->addItem("D3D11_BLEND_INV_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC1_ALPHA);
			if (desc.RenderTarget[0].DestBlendAlpha <= 11)
			{
				comboBox_DestBlendAlpha->setCurrentIndex(desc.RenderTarget[0].DestBlendAlpha - 1);
			}
			else
			{
				comboBox_DestBlendAlpha->setCurrentIndex(desc.RenderTarget[0].DestBlendAlpha - 3);
			}
			QHBoxLayout* hBoxLayout_DestBlendAlpha = new QHBoxLayout;
			QLabel* label_DestBlendAlpha = new QLabel("DestBlendAlpha:");
			hBoxLayout_DestBlendAlpha->addWidget(label_DestBlendAlpha);
			hBoxLayout_DestBlendAlpha->addWidget(comboBox_DestBlendAlpha);

			QComboBox* comboBox_BlendOpAlpha = new QComboBox;
			comboBox_BlendOpAlpha->addItem("D3D11_BLEND_OP_ADD", D3D11_BLEND_OP::D3D11_BLEND_OP_ADD);
			comboBox_BlendOpAlpha->addItem("D3D11_BLEND_OP_SUBTRACT", D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT);
			comboBox_BlendOpAlpha->addItem("D3D11_BLEND_OP_REV_SUBTRACT", D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT);
			comboBox_BlendOpAlpha->addItem("D3D11_BLEND_OP_MIN", D3D11_BLEND_OP::D3D11_BLEND_OP_MIN);
			comboBox_BlendOpAlpha->addItem("D3D11_BLEND_OP_MAX", D3D11_BLEND_OP::D3D11_BLEND_OP_MAX);
			comboBox_BlendOpAlpha->setCurrentIndex(desc.RenderTarget[0].BlendOpAlpha - 1);
			QHBoxLayout* hBoxLayout_BlendOpAlpha = new QHBoxLayout;
			QLabel* label_BlendOpAlpha = new QLabel("BlendOpAlpha:");
			hBoxLayout_BlendOpAlpha->addWidget(label_BlendOpAlpha);
			hBoxLayout_BlendOpAlpha->addWidget(comboBox_BlendOpAlpha);

			QSpinBox* spinBox_RenderTargetWriteMask = new QSpinBox(tableWidget);
			spinBox_RenderTargetWriteMask->setMaximum(255);
			spinBox_RenderTargetWriteMask->setMinimum(0);
			spinBox_RenderTargetWriteMask->setValue(desc.RenderTarget[0].RenderTargetWriteMask);
			QHBoxLayout* hBoxLayout_RenderTargetWriteMask = new QHBoxLayout();
			QLabel* label_RenderTargetWriteMask = new QLabel("RenderTargetWriteMask:");
			hBoxLayout_RenderTargetWriteMask->addWidget(label_RenderTargetWriteMask);
			hBoxLayout_RenderTargetWriteMask->addWidget(spinBox_RenderTargetWriteMask);

			auto func =
				[=]()
			{
				D3D11_BLEND_DESC tmpDesc;
				tmpDesc.AlphaToCoverageEnable = checkBox_AlphaToCoverageEnable->isChecked();
				tmpDesc.IndependentBlendEnable = checkBox_IndependentBlendEnable->isChecked();
				tmpDesc.RenderTarget[0].BlendEnable = checkBox_BlendEnable->isChecked();
				tmpDesc.RenderTarget[0].SrcBlend = comboBox_SrcBlend->currentData().value<D3D11_BLEND>();
				tmpDesc.RenderTarget[0].DestBlend = comboBox_DestBlend->currentData().value<D3D11_BLEND>();
				tmpDesc.RenderTarget[0].BlendOp = comboBox_BlendOp->currentData().value<D3D11_BLEND_OP>();
				tmpDesc.RenderTarget[0].SrcBlendAlpha = comboBox_SrcBlendAlpha->currentData().value<D3D11_BLEND>();
				tmpDesc.RenderTarget[0].DestBlendAlpha = comboBox_DestBlendAlpha->currentData().value<D3D11_BLEND>();
				tmpDesc.RenderTarget[0].BlendOpAlpha = comboBox_BlendOpAlpha->currentData().value<D3D11_BLEND_OP>();
				tmpDesc.RenderTarget[0].RenderTargetWriteMask = spinBox_RenderTargetWriteMask->value();
				prop.set_value(reflection, tmpDesc);
			};
			connect(checkBox_AlphaToCoverageEnable, &QCheckBox::stateChanged, this, func);
			connect(checkBox_IndependentBlendEnable, &QCheckBox::stateChanged, this, func);
			connect(checkBox_BlendEnable, &QCheckBox::stateChanged, this, func);
			connect(comboBox_SrcBlend, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_DestBlend, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_BlendOp, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_SrcBlendAlpha, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_DestBlendAlpha, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(comboBox_BlendOpAlpha, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, func);
			connect(spinBox_RenderTargetWriteMask, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, func);
			vBoxLayout->addLayout(hBoxLayout_AlphaToCoverageEnable);
			vBoxLayout->addLayout(hBoxLayout_IndependentBlendEnable);
			vBoxLayout->addLayout(hBoxLayout_BlendEnable);
			vBoxLayout->addLayout(hBoxLayout_SrcBlend);
			vBoxLayout->addLayout(hBoxLayout_DestBlend);
			vBoxLayout->addLayout(hBoxLayout_BlendOp);
			vBoxLayout->addLayout(hBoxLayout_SrcBlendAlpha);
			vBoxLayout->addLayout(hBoxLayout_DestBlendAlpha);
			vBoxLayout->addLayout(hBoxLayout_BlendOpAlpha);
			vBoxLayout->addLayout(hBoxLayout_RenderTargetWriteMask);

			widget->setLayout(vBoxLayout);
			tableWidget->setCellWidget(index, 1, widget);
		}
		else if (prop.get_type().get_name() == "classstd::shared_ptr<classDestiny::Material>")
		{
			using namespace Destiny;
			auto material = prop.get_value(reflection).get_value<std::shared_ptr<Material>>();

			QWidget* widget = new QWidget();
			QVBoxLayout* vBoxLayout = new QVBoxLayout();
			widget->setLayout(vBoxLayout);

			reflect(material, vBoxLayout, true);
			tableWidget->setCellWidget(index, 1, widget);
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





