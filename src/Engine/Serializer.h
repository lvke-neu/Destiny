#pragma once
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <rttr/registration>
#include <memory>
#include <string>

namespace Destiny
{
	class Object;
	class Serializer
	{
	public:
		static void Serialize(std::string& jsonStr, std::shared_ptr<Object> object);
	private:
		static void SerializeProperty(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	};
}