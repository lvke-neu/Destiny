#pragma once

//*************************************************singleton*************************************************
#define SINGLETON(CLASS) \
	public:\
		static CLASS* GetInstance()\
		{\
			static CLASS s_instance;\
			return &s_instance; \
		}
//********************************************************************************************************



//*************************************************log*************************************************
#include "Engine/Engine.h"
#include "Engine/LogManager.h"

#define LOG_TRACE(...)     Engine::GetInstance()->getLogManager()->getCoreLogger()->trace("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
#define LOG_INFO(...)      Engine::GetInstance()->getLogManager()->getCoreLogger()->info("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
#define LOG_WARN(...)      Engine::GetInstance()->getLogManager()->getCoreLogger()->warn("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
#define LOG_ERROR(...)     Engine::GetInstance()->getLogManager()->getCoreLogger()->error("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
#define LOG_CRITICAL(...)  Engine::GetInstance()->getLogManager()->getCoreLogger()->critical("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
//********************************************************************************************************

//*************************************************delete and release*************************************************
#define SAFE_DELETE(p) { if ((p)) { delete (p); (p) = nullptr; } } 
#define SAFE_RELEASE(p) { if ((p)) { (p)->Release(); (p) = nullptr; } }
//*********************************************************************************************************************

namespace Destiny
{
	class Utility
	{
	public:
		static std::wstring MultiByte2WideChar(const std::string& pKey);
		static std::string WideChar2MultiByte(const std::wstring& pWCStrKey);
		static std::string GenerateUUID();
	};
}

//*************************************************get set************************************************
#define GET(TYPE, NAME)\
	TYPE get_##NAME() const\
	{\
		return m_##NAME;\
	}\

#define SET(TYPE, NAME)\
	void set_##NAME(TYPE NAME)\
	{\
		m_##NAME = NAME;\
	}

#define GET_SET(TYPE, NAME)\
	GET(TYPE, NAME);\
	SET(TYPE, NAME);

#define GET_CLASS_NAME(NAME)\
	virtual std::string get_class_name() const\
	{\
		return #NAME;\
	}\
//**********************************************************************************************************