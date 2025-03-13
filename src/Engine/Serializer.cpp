#include "Serializer.h"
#include "Object.h"
#include "Scene.h"
#include "Node.h"
#include "Component.h"
#include "Math/Color.h"
#include "Math/Transform.h"
#include <DirectXMath.h>
#include <d3d11.h>

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
		writer.Key("type");
		writer.String(type.get_name().data());
		for (const auto& property : type.get_properties())
		{
			SerializeProperty(writer, property, object);
		}

		auto node = std::dynamic_pointer_cast<Destiny::Node>(object);
		if (node)
		{
			writer.Key("nodes");
			writer.StartArray();
			for (const auto& node : node->getChilds())
			{
				Serialize(writer, node);
			}
			writer.EndArray();

			writer.Key("components");
			writer.StartArray();
			for (const auto& component : node->getComponents())
			{
				Serialize(writer, component);
			}
			writer.EndArray();
		}

		writer.EndObject();
		jsonStr = buf.GetString();
	}
	
	void Serializer::Serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::shared_ptr<Object> object)
	{
		writer.StartObject();

		auto type = rttr::type::get(*object.get());
		writer.Key("type");
		writer.String(type.get_name().data());
		for (const auto& property : type.get_properties())
		{
			SerializeProperty(writer, property, object);
		}

		auto node = std::dynamic_pointer_cast<Destiny::Node>(object);
		if (node)
		{
			writer.Key("components");
			writer.StartArray();
			for (const auto& component : node->getComponents())
			{
				Serialize(writer, component);
			}
			writer.EndArray();
		}

		writer.EndObject();
	}

	void Serializer::SerializeProperty(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if(property.get_type() == rttr::type::get<bool>())
		{
			SerializeBool(writer, property, object);
		}
		else if(property.get_type() == rttr::type::get<std::string>())
		{
			SerializeString(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<float>())
		{
			SerializeFloat(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT2>())
		{
			SerializeFloat2(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT3>())
		{
			SerializeFloat3(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<Destiny::Color>())
		{
			SerializeColor(writer, property, object);
		}
		else if (property.get_type() == rttr::type::get<Destiny::Transform>())
		{
			SerializeTransform(writer, property, object);
		}
		else if (property.get_type().is_enumeration())
		{
			SerializeEnumeration(writer, property, object);
		}
	}

	void Serializer::SerializeBool(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		writer.Key(property.get_name().to_string().c_str());
		writer.Bool(property.get_value(object).to_bool());
	}

	void Serializer::SerializeString(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		writer.Key(property.get_name().to_string().c_str());
		writer.String(property.get_value(object).to_string().c_str());
	}

	void Serializer::SerializeFloat(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		writer.Key(property.get_name().to_string().c_str());
		writer.Double(property.get_value(object).to_float());
	}

	void Serializer::SerializeFloat2(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		DirectX::XMFLOAT2 value;
		property.get_value(object).convert(value);

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();
		writer.Key("x");
		writer.Double(value.x);
		writer.Key("y");
		writer.Double(value.y);
		writer.EndObject();
	}

	void Serializer::SerializeFloat3(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		DirectX::XMFLOAT3 value;
		property.get_value(object).convert(value);

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();
		writer.Key("x");
		writer.Double(value.x);
		writer.Key("y");
		writer.Double(value.y);
		writer.Key("z");
		writer.Double(value.z);
		writer.EndObject();
	}

	void Serializer::SerializeColor(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		Destiny::Color value;
		property.get_value(object).convert(value);

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();
		writer.Key("r");
		writer.Double(value.get_r());
		writer.Key("g");
		writer.Double(value.get_g());
		writer.Key("b");
		writer.Double(value.get_b());
		writer.Key("a");
		writer.Double(value.get_a());
		writer.EndObject();
	}

	void Serializer::SerializeTransform(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		Destiny::Transform value;
		property.get_value(object).convert(value);

		writer.Key(property.get_name().to_string().c_str());

		writer.StartObject();

		auto translation = value.get_translation();
		writer.Key("translation");
		writer.StartObject();
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
		writer.Key("x");
		writer.Double(rotation.x);
		writer.Key("y");
		writer.Double(rotation.y);
		writer.Key("z");
		writer.Double(rotation.z);
		writer.EndObject();

		writer.EndObject();
	}

	void Serializer::SerializeEnumeration(rapidjson::Writer<rapidjson::StringBuffer>& writer, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		auto enumeration = property.get_enumeration();
		std::vector<std::string> items;
		for (const auto& value : enumeration.get_values())
		{
			items.push_back(value.to_string());
		}
		auto itemIndex = (int)std::distance(items.begin(), std::find(items.begin(), items.end(), property.get_value(object).to_string()));

		writer.Key(property.get_name().to_string().c_str());
		writer.String(items[itemIndex].c_str());
	}
}