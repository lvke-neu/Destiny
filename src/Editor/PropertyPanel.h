#pragma once
#include "Meta.h"
#include <rttr/property.h>
#include <DirectXMath.h>
#include <d3d11.h>

namespace Destiny
{
	class Node;
	class Object;
}

class PropertyPanel : public Meta
{
public:
	PropertyPanel();
public:
	void update();
public slots:
	void onChoosedNode(void* parameter);
private:
	void reflect(std::shared_ptr<Destiny::Object> object);
	void reflectProperty(const rttr::property& property, std::shared_ptr<Destiny::Object> object);

	void reflectBool(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectString(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectFloat(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectFloat2(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectFloat3(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectColor(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectTransform(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectEnumeration(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectRasterizerDesc(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectDepthStencilStateDesc(const rttr::property& property, std::shared_ptr<Destiny::Object> object);
	void reflectBlendDesc(const rttr::property& property, std::shared_ptr<Destiny::Object> object);

	bool reflectDepthStencilOpDesc(const std::string& name, D3D11_DEPTH_STENCILOP_DESC& desc);
	bool reflectRendertargetBlendDesc(D3D11_RENDER_TARGET_BLEND_DESC& desc);
	bool reflectFloat(const rttr::property& property, float& value);
	bool reflectFloat3(const rttr::property& property, DirectX::XMFLOAT3& value);
	bool reflectEnumeration(const rttr::property& property, int& value, const std::string& name = "");
	bool reflectBool(const rttr::property& property, bool& value);
	bool reflectInt(const rttr::property& property, int& value, float speed = 1.0f, int min = 0, int max = 0);
private:
	std::shared_ptr<Destiny::Node> m_choosedNode;
};
