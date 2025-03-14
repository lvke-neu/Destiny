#include "UnSerializer.h"
#include "Object.h"
#include "Engine/Node.h"
#include "Engine/Component.h"
#include "Math/Color.h"
#include "Math/Transform.h"
#include "Graphics/Material.h"
#include <DirectXMath.h>
#include <d3d11.h>

#undef GetObject

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

				EXECUTION_TIME2(doc["type"].GetString(),
					for (const auto& property : type.get_properties())
					{
						UnSerializeProperty(doc, property, object);
					}

					auto node = std::dynamic_pointer_cast<Destiny::Node>(object);
					if (node)
					{
						UnSerializeNode(node, doc);
						UnSerializeComponent(node, doc);
						LOG_INFO("-----nodeName:{0}", node->get_name());
					}
				);
		

			}
		}
	}

	void UnSerializer::UnSerializeNode(std::shared_ptr<Node>& node, const rapidjson::Document& doc)
	{
		if (doc.HasMember("nodes") && doc["nodes"].IsArray())
		{
			for (unsigned int i = 0; i < doc["nodes"].Size(); i++)
			{
				if (doc["nodes"][i].IsObject())
				{
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

					const rapidjson::Value& rapidjsonValue = doc["nodes"][i].GetObject();
					rapidjsonValue.Accept(writer);

					std::shared_ptr<Object> object = nullptr;
					UnSerialize(object, buffer.GetString());
					std::static_pointer_cast<Node>(object)->addToParent(node);
				}
			}
		}
	}

	void UnSerializer::UnSerializeComponent(std::shared_ptr<Node>& node, const rapidjson::Document& doc)
	{
		if (doc.HasMember("components") && doc["components"].IsArray())
		{
			for (unsigned int i = 0; i < doc["components"].Size(); i++)
			{
				if (doc["components"][i].IsObject())
				{
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
						
					const rapidjson::Value& rapidjsonValue = doc["components"][i].GetObject();
					rapidjsonValue.Accept(writer);

					std::shared_ptr<Object> object = nullptr;
					UnSerialize(object, buffer.GetString());

					node->addComponent(std::static_pointer_cast <Component>(object));
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
			UnSerializeFloat2(doc, property, object);
		}
		else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT3>())
		{
			UnSerializeFloat3(doc, property, object);
		}
		else if (property.get_type() == rttr::type::get<Destiny::Color>())
		{
			UnSerializeColor(doc, property, object);
		}
		else if (property.get_type() == rttr::type::get<Destiny::Transform>())
		{
			UnSerializeTransform(doc, property, object);
		}
		else if (property.get_type().is_enumeration())
		{
			UnSerializeEnumeration(doc, property, object);
		}
		else if (property.get_type() == rttr::type::get<std::shared_ptr<Destiny::Material>>())
		{
			UnSerializeMaterial(doc, property, object);
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

	void UnSerializer::UnSerializeFloat2(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if (doc.HasMember(property.get_name().to_string().c_str()) && doc[property.get_name().to_string().c_str()].IsObject())
		{
			DirectX::XMFLOAT2 value;
			value.x = doc[property.get_name().to_string().c_str()]["x"].GetFloat();
			value.y = doc[property.get_name().to_string().c_str()]["y"].GetFloat();

			property.set_value(object, value);
		}
	}

	void UnSerializer::UnSerializeFloat3(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if (doc.HasMember(property.get_name().to_string().c_str()) && doc[property.get_name().to_string().c_str()].IsObject())
		{
			DirectX::XMFLOAT3 value;
			value.x = doc[property.get_name().to_string().c_str()]["x"].GetFloat();
			value.y = doc[property.get_name().to_string().c_str()]["y"].GetFloat();
			value.z = doc[property.get_name().to_string().c_str()]["z"].GetFloat();

			property.set_value(object, value);
		}
	}

	void UnSerializer::UnSerializeColor(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if (doc.HasMember(property.get_name().to_string().c_str()) && doc[property.get_name().to_string().c_str()].IsObject())
		{
			Destiny::Color value;
			value.set_r(doc[property.get_name().to_string().c_str()]["r"].GetFloat());
			value.set_g(doc[property.get_name().to_string().c_str()]["g"].GetFloat());
			value.set_b(doc[property.get_name().to_string().c_str()]["b"].GetFloat());
			value.set_a(doc[property.get_name().to_string().c_str()]["a"].GetFloat());

			property.set_value(object, value);
		}
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

	void UnSerializer::UnSerializeEnumeration(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		if (doc.HasMember(property.get_name().to_string().c_str()) && doc[property.get_name().to_string().c_str()].IsString())
		{
			auto value = property.get_enumeration().name_to_value(doc[property.get_name().to_string().c_str()].GetString());
			property.set_value(object, value);
		}
	}

	void UnSerializer::UnSerializeMaterial(const rapidjson::Document& doc, const rttr::property& property, std::shared_ptr<Destiny::Object> object)
	{
		std::shared_ptr<Destiny::Object> material = nullptr;

		if (doc.HasMember("material") && doc["material"].IsObject())
		{
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

			const rapidjson::Value& rapidjsonValue = doc["material"].GetObject();
			rapidjsonValue.Accept(writer);

			UnSerialize(material, buffer.GetString());
		}

		property.set_value(object, std::dynamic_pointer_cast<Destiny::Material>(material));
	}
}