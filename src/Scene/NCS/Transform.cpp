#include "Transform.h"

namespace Destiny
{
	Transform::Transform() :
		m_translation(0.0f, 0.0f, 0.0f),
		m_rotation(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f)
	{

	}

	DirectX::XMMATRIX Transform::getWorldMatrix() const
	{
		DirectX::XMFLOAT3 radiansRotation { DirectX::XMConvertToRadians(m_rotation.x), DirectX::XMConvertToRadians(m_rotation.y), DirectX::XMConvertToRadians(m_rotation.z) };
		return DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&m_scale)) *
			DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation)) *
			DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_translation));
	}

	DirectX::XMMATRIX Transform::getTransposeWorldMatrix() const
	{
		return DirectX::XMMatrixTranspose(getWorldMatrix());
	}

	DirectX::XMMATRIX Transform::getInvTransposeWorldMatrix() const
	{
		return  DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, getWorldMatrix()));
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Transform>("Transform")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
			)
		.property("translation", &Transform::get_translation, &Transform::set_translation)
		.property("rotation", &Transform::get_rotation, &Transform::set_rotation)
		.property("scale", &Transform::get_scale, &Transform::set_scale);
	}
}