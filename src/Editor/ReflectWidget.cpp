#include "ReflectWidget.h"
#include "Engine/Reflection.h"
#include <QVBoxLayout>
#include <QLabel>
#include <rttr/type>

namespace Destiny
{
	void ReflectWidget::Reflect(std::shared_ptr<Reflection> reflection, QVBoxLayout* layout)
	{
		if (!reflection)
		{
			return;
		}

		if (layout)
		{
			layout->addWidget(new QLabel(reflection->get_class_name().c_str(), nullptr));
		}
	}
}