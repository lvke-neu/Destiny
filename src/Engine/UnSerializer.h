#pragma once
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <rttr/registration>
#include <memory>
#include <string>

namespace Destiny
{
	class Object;
	class UnSerializer
	{
	public:
		static void UnSerialize(std::shared_ptr<Object>& object, const std::string& jsonStr);
	private:
		static void UnSerializeProperty(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);

		static void UnSerializeBool(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeString(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat2(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat3(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeColor(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeTransform(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeEnumeration(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	};
}