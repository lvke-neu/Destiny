#pragma once
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <rttr/registration>
#include <memory>
#include <string>

namespace Destiny
{
	class Object;
	class Scene;
	class Serializer
	{
	public:
		static void Serialize(std::string& jsonStr, std::shared_ptr<Object> object);

	private:
		static void Serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::shared_ptr<Object> object);

		static void SerializeProperty(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);

		static void SerializeBool(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void SerializeString(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void SerializeFloat(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void SerializeFloat2(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void SerializeFloat3(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void SerializeColor(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void SerializeTransform(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void SerializeEnumeration(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	};
}