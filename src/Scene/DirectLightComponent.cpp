#include "DirectLightComponent.h"
#include "Node3D.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	DirectLightComponent::DirectLightComponent()
	{
		m_direction = std::make_shared<ConstantBuffer<XMFLOAT4>>();
	}

	void DirectLightComponent::onAttachNode()
	{
		XMFLOAT3 dir = m_node->get_transform3D().getAtDirection();
		m_direction->update({ dir.x, dir.y, dir.z, 1.0f });
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetConstantBuffers(4, 1, m_direction->getConstantBuffer());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(4, 1, m_direction->getConstantBuffer());
	}

	void DirectLightComponent::onNodeTransformChanged()
	{
		XMFLOAT3 dir = m_node->get_transform3D().getAtDirection();
		m_direction->update({ dir.x, dir.y, dir.z, 1.0f });
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<DirectLightComponent>("DirectLightComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}