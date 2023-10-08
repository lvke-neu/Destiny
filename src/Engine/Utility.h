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
#define LOG_TRACE(...)     Engine::GetInstance()->getLogManager()->getCoreLogger()->trace("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
#define LOG_INFO(...)      Engine::GetInstance()->getLogManager()->getCoreLogger()->info("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
#define LOG_WARN(...)      Engine::GetInstance()->getLogManager()->getCoreLogger()->warn("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
#define LOG_ERROR(...)     Engine::GetInstance()->getLogManager()->getCoreLogger()->error("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
#define LOG_CRITICAL(...)  Engine::GetInstance()->getLogManager()->getCoreLogger()->critical("[" + std::string(__FUNCTION__) + "]:" + __VA_ARGS__);
//********************************************************************************************************
