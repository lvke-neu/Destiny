#include "Reflection.h"

namespace Destiny
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<Reflection>("Reflection")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}