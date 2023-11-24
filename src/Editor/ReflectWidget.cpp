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
			QLayoutItem* child;
			while ((child = layout->takeAt(0)) != 0) {
				if (child->widget())
				{
					child->widget()->setParent(nullptr);
					delete child->widget();
				}	
				delete child;
			}
			
			layout->addWidget(new QLabel(reflection->get_class_name().c_str(), nullptr));

			rttr::type type = rttr::type::get_by_name(reflection->get_class_name());

			for (auto& prop : type.get_properties())
			{
				layout->addWidget(new QLabel(prop.get_name().to_string().c_str(), nullptr));

				if (prop.get_type().get_name() == "classstd::basic_string<char,structstd::char_traits<char>,classstd::allocator<char> >")
				{
					std::string str = prop.get_value(reflection).to_string();

					layout->addWidget(new QLabel(str.c_str(), nullptr));
				}
			
			}
		}
	}
}