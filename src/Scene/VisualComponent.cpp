#include "VisualComponent.h"

namespace Destiny
{
	VisualComponent::VisualComponent() : 
		m_visual(std::make_shared<Visual>())
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualComponent>("VisualComponent")
			.constructor<>();
	}
}