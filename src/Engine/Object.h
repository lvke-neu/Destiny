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
		Object();
		virtual ~Object() = default;
	public:
		GET(std::string, uuid);
		GET_SET(bool, serializable);
	private:
		std::string		m_uuid;
		bool			m_serializable;
	};
}