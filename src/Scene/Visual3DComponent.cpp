#include "Visual3DComponent.h"
#include "Graphics/Visual3D.h"

namespace Destiny
{
	Visual3DComponent::Visual3DComponent() : 
		m_visual3D(nullptr)
	{

	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Visual3DComponent>("Visual3DComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}