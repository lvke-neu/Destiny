#include "Transform.h"
#include "Math.h"

namespace Destiny
{
	Transform::Transform() :
		m_translation(0.0f, 0.0f, 0.0f),
		m_rotation(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f),
		m_worldMatrix(DirectX::XMMatrixIdentity())
	{

	}

	DirectX::XMMATRIX Transform::getWorldMatrix() const
	{
		return m_worldMatrix;
	}

	void Transform::setWorldMatrix(const DirectX::XMMATRIX& worldMatrix)
	{
		m_worldMatrix = worldMatrix;

		float arr[4][4];
		memcpy_s(arr, sizeof(arr), &worldMatrix, sizeof(arr));
		m_rotation.x = RAD2DEG * atan2f(arr[1][2], arr[2][2]);
		m_rotation.y = RAD2DEG* atan2f(-arr[0][2], sqrtf(arr[1][2] * arr[1][2] + arr[2][2] * arr[2][2]));
		m_rotation.z = RAD2DEG * atan2f(arr[0][1], arr[0][0]);
	}

	DirectX::XMMATRIX Transform::getTransposeWorldMatrix() const
	{
		return DirectX::XMMatrixTranspose(getWorldMatrix());
	}

	DirectX::XMMATRIX Transform::getInverseWorldMatrix() const
	{
		return DirectX::XMMatrixInverse(nullptr, getWorldMatrix());
	}

	DirectX::XMMATRIX Transform::getInvTransposeWorldMatrix() const
	{
		return  DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, getWorldMatrix()));
	}

	void Transform::set_translation(DirectX::XMFLOAT3 translation)
	{
		m_translation = translation;
		updateWorldMatrix();
	}

	void Transform::set_rotation(DirectX::XMFLOAT3 rotation)
	{
		m_rotation = rotation;
		updateWorldMatrix();
	}

	void Transform::set_scale(DirectX::XMFLOAT3 scale)
	{
		m_scale = scale;
		updateWorldMatrix();
	}

	void Transform::moveZAxis(float distance)
	{
		DirectX::XMFLOAT3 radiansRotation{ DirectX::XMConvertToRadians(m_rotation.x), DirectX::XMConvertToRadians(m_rotation.y), DirectX::XMConvertToRadians(m_rotation.z) };
		DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation));

		DirectX::XMVECTOR pos = XMLoadFloat3(&m_translation);
		DirectX::XMVECTOR dir = rotMatrix.r[2];
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(dir, distance));

		XMStoreFloat3(&m_translation, pos);

		updateWorldMatrix();
	}

	void Transform::moveXAxis(float distance)
	{
		DirectX::XMFLOAT3 radiansRotation{ DirectX::XMConvertToRadians(m_rotation.x), DirectX::XMConvertToRadians(m_rotation.y), DirectX::XMConvertToRadians(m_rotation.z) };
		DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation));

		DirectX::XMVECTOR pos = XMLoadFloat3(&m_translation);
		DirectX::XMVECTOR dir = rotMatrix.r[0];
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(dir, distance));

		XMStoreFloat3(&m_translation, pos);

		updateWorldMatrix();
	}

	void Transform::rotateXAxis(float angle)
	{
		m_rotation.x -= angle;
		updateWorldMatrix();
	}

	void Transform::rotateYAxis(float angle)
	{
		m_rotation.y -= angle;
		updateWorldMatrix();
	}

	void Transform::updateWorldMatrix()
	{
		DirectX::XMFLOAT3 radiansRotation{ DirectX::XMConvertToRadians(m_rotation.x), DirectX::XMConvertToRadians(m_rotation.y), DirectX::XMConvertToRadians(m_rotation.z) };
		m_worldMatrix =
			DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&m_scale)) *
			DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation)) *
			DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_translation));
	}

	std::string Transform::toString()
	{
		DirectX::XMFLOAT4 r0;
		DirectX::XMFLOAT4 r1;
		DirectX::XMFLOAT4 r2;
		DirectX::XMFLOAT4 r3;
		DirectX::XMStoreFloat4(&r0, m_worldMatrix.r[0]);
		DirectX::XMStoreFloat4(&r1, m_worldMatrix.r[1]);
		DirectX::XMStoreFloat4(&r2, m_worldMatrix.r[2]);
		DirectX::XMStoreFloat4(&r3, m_worldMatrix.r[3]);
		return "scale_x:" +std::to_string(m_scale.x) + "\nscale_y:" + std::to_string(m_scale.y) + "\nscale_z:" + std::to_string(m_scale.z) + 
			"\nrotation_x:" + std::to_string(m_rotation.x) + "\nrotation_y:" + std::to_string(m_rotation.y) + "\nrotation_z:" + std::to_string(m_rotation.z) +
			"\ntranslation_x:" + std::to_string(m_translation.x) + "\ntranslation_y:" + std::to_string(m_translation.y) + "\ntranslation_z:" + std::to_string(m_translation.z) + 
			"WorldMatrix:\n"+
			std::to_string(r0.x) + "," + std::to_string(r0.y) + ","+ std::to_string(r0.z) + ","+ std::to_string(r0.w) + "\n" +
			std::to_string(r1.x) + "," + std::to_string(r1.y) + ","+ std::to_string(r1.z) + ","+ std::to_string(r1.w) + "\n" +
			std::to_string(r2.x) + "," + std::to_string(r2.y) + ","+ std::to_string(r2.z) + ","+ std::to_string(r2.w) + "\n" +
			std::to_string(r3.x) + "," + std::to_string(r3.y) + ","+ std::to_string(r3.z) + ","+ std::to_string(r3.w) + "\n";
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