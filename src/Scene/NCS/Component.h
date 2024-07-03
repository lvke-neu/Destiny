#pragma once
#include "Engine/Object.h"

namespace Destiny
{
	class Component : public Object
	{
		RTTR_ENABLE(Object);
	public:
		Component();
		virtual ~Component() = default;
	public:
		GET_CLASS_NAME(Component);
	};
}