#include "PointLightComponent.h"
#include "Node3D.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Material.h"

namespace Destiny
{
	PointLightComponent::PointLightComponent()
	{
		m_direction = std::make_shared<ConstantBuffer<cbPointLight>>();

		m_material->set_useColor(true);
		m_material->set_ambientColor(Color::White);
		m_material->load();
	}

	void PointLightComponent::onAttachNode()
	{
		Visual3DComponent::onAttachNode();

		cbPointLight pointLight;
		
		XMFLOAT3 pos = m_node->get_transform3D().get_translation();
		pointLight.position = { pos.x, pos.y, pos.z, 1.0f };
		pointLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_direction->update(pointLight);

		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetConstantBuffers(5, 1, m_direction->getConstantBuffer());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(5, 1, m_direction->getConstantBuffer());
	}

	void PointLightComponent::onNodeTransformChanged()
	{
		Visual3DComponent::onNodeTransformChanged();

		cbPointLight pointLight;

		XMFLOAT3 pos = m_node->get_transform3D().get_translation();
		pointLight.position = { pos.x, pos.y, pos.z, 1.0f };
		pointLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_direction->update(pointLight);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<PointLightComponent>("PointLightComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}