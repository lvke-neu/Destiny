#include "GraphicsDefine.h"
#include <rttr/registration.h>

namespace Destiny
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<RendererCategory>("RendererCategory")
		.enumeration<RendererCategory>("None")
		.enumeration<RendererCategory>("ForwardOpaque")
		.enumeration<RendererCategory>("ForwardTransparent")
		.enumeration<RendererCategory>("Gui");
	}
}