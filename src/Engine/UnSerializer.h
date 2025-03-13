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
	class Node;
	class UnSerializer
	{
	public:
		static void UnSerialize(std::shared_ptr<Object>& object, const std::string& jsonStr);
		static void UnSerializeNode(std::shared_ptr<Node>& node, const rapidjson::Document& doc);
		static void UnSerializeComponent(std::shared_ptr<Node>& node, const rapidjson::Document& doc);
	private:
		static void UnSerializeProperty(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);

		static void UnSerializeBool(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeString(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat2(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeFloat3(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeColor(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeTransform(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
		static void UnSerializeEnumeration(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	};
}