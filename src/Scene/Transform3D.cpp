#include "Transform3D.h"

namespace Destiny
{
	Transform3D::Transform3D() :
		m_translation(0.0f, 0.0f, 0.0f),
		m_rotation(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f)
	{

	}

	XMMATRIX Transform3D::getWorldMatrix()
	{
		XMFLOAT3 radiansRotation { XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z) };
		return XMMatrixScalingFromVector(XMLoadFloat3(&m_scale)) *
			XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&m_translation));
	}

	XMFLOAT3 Destiny::Transform3D::getAtDirection()
	{
		XMFLOAT3 radiansRotation{ XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z) };
		XMMATRIX rotMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation));
		XMFLOAT3 dir;
		XMStoreFloat3(&dir, XMVector3Normalize(rotMatrix.r[2]));

		return XMFLOAT3(0.0f, -0.5f, 0.5f);
	}

	void Transform3D::moveZAxis(float distance)
	{
		XMFLOAT3 radiansRotation{ XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z) };
		XMMATRIX rotMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation));

		XMVECTOR pos = XMLoadFloat3(&m_translation);
		XMVECTOR dir = rotMatrix.r[2];
		pos += dir * distance;

		XMStoreFloat3(&m_translation, pos);
	}

	void Transform3D::moveXAxis(float distance)
	{
		XMFLOAT3 radiansRotation{ XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z) };
		XMMATRIX rotMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation));

		XMVECTOR pos = XMLoadFloat3(&m_translation);
		XMVECTOR dir = rotMatrix.r[0];
		pos += dir * distance;

		XMStoreFloat3(&m_translation, pos);
	}

	void Transform3D::rotateXAxis(float angle)
	{
		m_rotation.x -= angle;
	}
	void Transform3D::rotateYAxis(float angle)
	{
		m_rotation.y -= angle;
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<XMFLOAT3>("XMFLOAT3")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
			.property("x", &XMFLOAT3::x)
			.property("y", &XMFLOAT3::y)
			.property("z", &XMFLOAT3::z);

		rttr::registration::class_<Transform3D>("Transform3D")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
			)
		.property("translation", &Transform3D::get_translation, &Transform3D::set_translation)
		.property("rotation", &Transform3D::get_rotation, &Transform3D::set_rotation)
		.property("scale", &Transform3D::get_scale, &Transform3D::set_scale);
	}
}