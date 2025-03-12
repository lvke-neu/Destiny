#include "UnSerializer.h"
#include "Object.h"
#include "Math/Color.h"
#include "Math/Transform.h"
#include <DirectXMath.h>
#include <d3d11.h>

namespace Destiny
{
	void UnSerializer::UnSerialize(std::shared_ptr<Object>& object, const std::string& jsonStr)
	{
		rapidjson::Document doc;
		doc.Parse(jsonStr.c_str());
		if (!doc.HasParseError())
		{
			if (doc.HasMember("type") && doc["type"].IsString())
			{
				rttr::type type = rttr::type::get_by_name(doc["type"].GetString());
				if (type.is_valid())
				{
					rttr::variant variant = type.create();
					if (variant.is_valid())
					{
						object = variant.get_value<std::shared_ptr<Object>>();
					}
				}


				for (const auto& property : type.get_properties())
				{
					UnSerializeProperty(doc, property, object);
				}
			}
		}
	}
	
	void UnSerializer::UnSerializeProperty(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if(property.get_type() == rttr::type::get<bool>())
		{
			UnSerializeBool(doc, property, object);
		}
		else if(property.get_type() == rttr::type::get<std::string>())
		{
			UnSerializeString(doc, property, object);
		}
		else if (property.get_type() == rttr::type::get<float>())
		{
			UnSerializeFloat(doc, property, object);
		}
		else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT2>())
		{
			//UnSerializeFloat2(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT3>())
		{
			//UnSerializeFloat3(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<Destiny::Color>())
		{
			//UnSerializeColor(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<Destiny::Transform>())
		{
			UnSerializeTransform(doc, property, object);
		}
		else if (property.get_type().is_enumeration())
		{
			//UnSerializeEnumeration(writer, property, object);
		}
	}

	void UnSerializer::UnSerializeBool(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if (doc.HasMember(property.get_name().to_string().c_str()) && doc[property.get_name().to_string().c_str()].IsBool())
		{
			property.set_value(object, doc[property.get_name().to_string().c_str()].GetBool());
		}
	}

	void UnSerializer::UnSerializeString(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if (doc.HasMember(property.get_name().to_string().c_str()) && doc[property.get_name().to_string().c_str()].IsString())
		{
			property.set_value(object, std::string(doc[property.get_name().to_string().c_str()].GetString()));
		}
	}

	void UnSerializer::UnSerializeFloat(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if (doc.HasMember(property.get_name().to_string().c_str()) && doc[property.get_name().to_string().c_str()].IsFloat())
		{
			property.set_value(object, doc[property.get_name().to_string().c_str()].GetFloat());
		}
	}

	void UnSerializer::UnSerializeFloat2(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		DirectX::XMFLOAT2 value;
		property.get_value(object).convert(value);

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();
		writer.Key("type");
		writer.String("DirectX::XMFLOAT2");
		writer.Key("x");
		writer.Double(value.x);
		writer.Key("y");
		writer.Double(value.y);
		writer.EndObject();
	}

	void UnSerializer::UnSerializeFloat3(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		DirectX::XMFLOAT3 value;
		property.get_value(object).convert(value);

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();
		writer.Key("type");
		writer.String("DirectX::XMFLOAT3");
		writer.Key("x");
		writer.Double(value.x);
		writer.Key("y");
		writer.Double(value.y);
		writer.Key("z");
		writer.Double(value.z);
		writer.EndObject();
	}

	void UnSerializer::UnSerializeColor(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		Destiny::Color value;
		property.get_value(object).convert(value);

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();
		writer.Key("type");
		writer.String("Destiny::Color");
		writer.Key("r");
		writer.Double(value.get_r());
		writer.Key("g");
		writer.Double(value.get_g());
		writer.Key("b");
		writer.Double(value.get_b());
		writer.Key("r");
		writer.Double(value.get_a());
		writer.EndObject();
	}

	void UnSerializer::UnSerializeTransform(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if (doc.HasMember(property.get_name().to_string().c_str()) && doc[property.get_name().to_string().c_str()].IsObject())
		{
			Destiny::Transform transform;
			
			transform.set_translation
			(
				{
					doc[property.get_name().to_string().c_str()]["translation"]["x"].GetFloat(),
					doc[property.get_name().to_string().c_str()]["translation"]["y"].GetFloat(),
					doc[property.get_name().to_string().c_str()]["translation"]["z"].GetFloat()
				}
			);
		
			transform.set_scale
			(
				{
					doc[property.get_name().to_string().c_str()]["scale"]["x"].GetFloat(),
					doc[property.get_name().to_string().c_str()]["scale"]["y"].GetFloat(),
					doc[property.get_name().to_string().c_str()]["scale"]["z"].GetFloat()
				}
			);

			transform.set_rotation
			(
				{
					doc[property.get_name().to_string().c_str()]["rotation"]["x"].GetFloat(),
					doc[property.get_name().to_string().c_str()]["rotation"]["y"].GetFloat(),
					doc[property.get_name().to_string().c_str()]["rotation"]["z"].GetFloat()
				}
			);

			property.set_value(object, transform);
		}
	}

	void UnSerializer::UnSerializeEnumeration(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		auto enumeration = property.get_enumeration();
		std::vector<std::string> items;
		for (const auto& value : enumeration.get_values())
		{
			items.push_back(value.to_string());
		}
		auto itemIndex = (int)std::distance(items.begin(), std::find(items.begin(), items.end(), property.get_value(object).to_string()));

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();

		writer.Key("type");
		writer.String(property.get_type().get_name().to_string().c_str());
		writer.Key("Value");
		writer.String(items[itemIndex].c_str());
		writer.EndObject();
	}
}