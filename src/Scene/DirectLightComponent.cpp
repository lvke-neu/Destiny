#include "DirectLightComponent.h"
#include "Node3D.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	DirectLightComponent::DirectLightComponent()
	{
		m_direction = std::make_shared<ConstantBuffer<cbDirectLight>>();
	}

	void DirectLightComponent::onAttachNode()
	{
		cbDirectLight directLight;
		XMFLOAT3 dir = m_node->get_transform3D().getAtDirection();
		directLight.direction = { dir.x, dir.y, dir.z, 1.0f };
		//directLight.view = XMMatrixTranspose(XMMatrixInverse(nullptr, m_node->get_transform3D().getWorldMatrix()));
		auto eyePos = m_node->get_transform3D().get_translation();
		directLight.view = XMMatrixTranspose(XMMatrixLookAtLH(XMLoadFloat3(&eyePos), g_XMZero, g_XMIdentityR1));
		directLight.proj = XMMatrixTranspose(XMMatrixOrthographicLH(40.0f, 40.0f, 1.0f, 100.0f));
		m_direction->update(directLight);

		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetConstantBuffers(4, 1, m_direction->getConstantBuffer());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(4, 1, m_direction->getConstantBuffer());
	}

	void DirectLightComponent::onNodeTransformChanged()
	{
		cbDirectLight directLight;
		XMFLOAT3 dir = m_node->get_transform3D().getAtDirection();
		directLight.direction = { dir.x, dir.y, dir.z, 1.0f };
		directLight.view = XMMatrixTranspose(XMMatrixInverse(nullptr, m_node->get_transform3D().getWorldMatrix()));
		directLight.proj = XMMatrixTranspose(XMMatrixOrthographicLH(40.0f, 40.0f, 1.0f, 100.0f));
		m_direction->update(directLight);
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