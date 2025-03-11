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
		//if (!object)
		//{
		//	return;
		//}

		//rapidjson::StringBuffer buf;
		//rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
		//writer.StartObject();

		//auto type = rttr::type::get(*object.get());
		//writer.Key("type");
		//writer.String(type.get_name().data());
		//for (const auto& property : type.get_properties())
		//{
		//	UnSerializeProperty(writer, property, object);
		//}

		//writer.EndObject();
		//jsonStr = buf.GetString();
	}
	
	void UnSerializer::UnSerializeProperty(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if(property.get_type() == rttr::type::get<bool>())
		{
			UnSerializeBool(writer, property, object);
		}
		else if(property.get_type() == rttr::type::get<std::string>())
		{
			UnSerializeString(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<float>())
		{
			UnSerializeFloat(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT2>())
		{
			UnSerializeFloat2(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT3>())
		{
			UnSerializeFloat3(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<Destiny::Color>())
		{
			UnSerializeColor(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<Destiny::Transform>())
		{
			UnSerializeTransform(writer, property, object);
		}
		else if (property.get_type().is_enumeration())
		{
			UnSerializeEnumeration(writer, property, object);
		}
	}

	void UnSerializer::UnSerializeBool(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		writer.Key(property.get_name().to_string().c_str());
		writer.Bool(property.get_value(object).to_bool());
	}

	void UnSerializer::UnSerializeString(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		writer.Key(property.get_name().to_string().c_str());
		writer.String(property.get_value(object).to_string().c_str());
	}

	void UnSerializer::UnSerializeFloat(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		writer.Key(property.get_name().to_string().c_str());
		writer.Double(property.get_value(object).to_float());
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

	void UnSerializer::UnSerializeTransform(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		Destiny::Transform value;
		property.get_value(object).convert(value);

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();
		
		writer.Key("type");
		writer.String("Destiny::Transform");

		auto translation = value.get_translation();
		writer.Key("translation");
		writer.StartObject();
		writer.Key("type");
		writer.String("DirectX::XMFLOAT3");
		writer.Key("x");
		writer.Double(translation.x);
		writer.Key("y");
		writer.Double(translation.y);
		writer.Key("z");
		writer.Double(translation.z);
		writer.EndObject();

		auto scale = value.get_scale();
		writer.Key("scale");
		writer.StartObject();
		writer.Key("type");
		writer.String("DirectX::XMFLOAT3");
		writer.Key("x");
		writer.Double(scale.x);
		writer.Key("y");
		writer.Double(scale.y);
		writer.Key("z");
		writer.Double(scale.z);
		writer.EndObject();

		auto rotation = value.get_rotation();
		writer.Key("rotation");
		writer.StartObject();
		writer.Key("type");
		writer.String("DirectX::XMFLOAT3");
		writer.Key("x");
		writer.Double(rotation.x);
		writer.Key("y");
		writer.Double(rotation.y);
		writer.Key("z");
		writer.Double(rotation.z);
		writer.EndObject();

		writer.EndObject();
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