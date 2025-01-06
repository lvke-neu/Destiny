#include "GraphicsDefine.h"
#include <rttr/registration>

namespace Destiny
{
	RTTR_REGISTRATION
	{

	rttr::registration::enumeration<RendererCategory>("RendererCategory")
		(
			rttr::value("None", RendererCategory::None),
			rttr::value("ForwardOpaque", RendererCategory::ForwardOpaque),
			rttr::value("ForwardTransparent", RendererCategory::ForwardTransparent),
			rttr::value("Gui", RendererCategory::Gui)
		);
	}
}