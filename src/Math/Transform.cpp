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

	Transform::Transform(const Transform& other) :
		m_translation(other.m_translation),
		m_rotation(other.m_rotation),
		m_scale(other.m_scale),
		m_worldMatrix(other.m_worldMatrix)
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

		//translation
		memcpy_s(&m_translation, sizeof(m_translation), arr[3], sizeof(m_translation));
		
		//rotation
		
		m_rotation.x = Math::RAD2DEG * atan2f(arr[1][2], arr[2][2]);
		m_rotation.y = Math::RAD2DEG* atan2f(-arr[0][2], sqrtf(arr[1][2] * arr[1][2] + arr[2][2] * arr[2][2]));
		m_rotation.z = Math::RAD2DEG * atan2f(arr[0][1], arr[0][0]);

		//scale
		DirectX::XMFLOAT3 radiansRotation{ DirectX::XMConvertToRadians(m_rotation.x), DirectX::XMConvertToRadians(m_rotation.y), DirectX::XMConvertToRadians(m_rotation.z) };
		auto rotationMatrixInverse = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radiansRotation)));
		auto translationMatrixInverse = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_translation)));
		auto scaleMatrix = m_worldMatrix * rotationMatrixInverse * translationMatrixInverse;
		memcpy_s(arr, sizeof(arr), &scaleMatrix, sizeof(arr));
		m_scale.x = arr[0][0];
		m_scale.y = arr[1][1];
		m_scale.z = arr[2][2];
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
		DirectX::XMFLOAT3 radianRotation{ DirectX::XMConvertToRadians(m_rotation.x), DirectX::XMConvertToRadians(m_rotation.y), DirectX::XMConvertToRadians(m_rotation.z) };
		m_worldMatrix =
			DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&m_scale)) *
			DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&radianRotation)) *
			DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_translation));
	}


	RTTR_REGISTRATION
	{
		rttr::registration::class_<Transform>("Transform")
		.constructor<>()
		.property("translation", &Transform::get_translation, &Transform::set_translation)
		.property("rotation", &Transform::get_rotation, &Transform::set_rotation)
		.property("scale", &Transform::get_scale, &Transform::set_scale);
	}
}