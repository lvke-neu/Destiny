#include "Serializer.h"
#include "Object.h"

namespace Destiny
{
	void Serializer::Serialize(std::string& jsonStr, std::shared_ptr<Object> object)
	{
		if (!object)
		{
			return;
		}

		rapidjson::StringBuffer buf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
		writer.StartObject();

		auto type = rttr::type::get(*object.get());
		for (const auto& property : type.get_properties())
		{
			SerializeProperty(writer, property, object);
		}

		writer.EndObject();
		jsonStr = buf.GetString();
	}
	
	void Serializer::SerializeProperty(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if(property.get_type() == rttr::type::get<bool>())
		{
			writer.Key(property.get_name().to_string().c_str());
			writer.Bool(property.get_value(object).to_bool());
		}
		else if(property.get_type() == rttr::type::get<std::string>())
		{
			writer.Key(property.get_name().to_string().c_str());
			writer.String(property.get_value(object).to_string().c_str());
		}
	}
}