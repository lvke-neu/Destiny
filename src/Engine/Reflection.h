#pragma once
#include "Engine/Utility.h"
#include <rttr/registration>

namespace Destiny
{
	class Reflection
	{
		RTTR_ENABLE();
	public:
		virtual ~Reflection() = default;
	public:
		GET_CLASS_NAME(Reference);
	};
}