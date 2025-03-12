#pragma once
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
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
		static void UnSerializeProperty(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);

		static void UnSerializeBool(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeString(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat2(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat3(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeColor(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeTransform(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeEnumeration(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	};
}