#pragma once
#undef max
#undef min
#include "Engine/Utility.h"
#include <rttr/registration>

namespace Destiny
{
	class Object
	{
		RTTR_ENABLE();
	public:
		virtual ~Object() = default;
	public:
		GET_CLASS_NAME(Object);
	};
}