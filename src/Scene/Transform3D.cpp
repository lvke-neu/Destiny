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
		return XMMatrixScalingFromVector(XMLoadFloat3(&m_scale)) *
			XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotation)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&m_translation));
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