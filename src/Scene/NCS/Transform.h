#pragma once
#include "Engine/Object.h"
#include <DirectXMath.h>

namespace Destiny
{
	class Transform : public Object
	{
		RTTR_ENABLE(Object);
	public:
		Transform();
	public:
		DirectX::XMMATRIX getWorldMatrix() const;
		DirectX::XMMATRIX getTransposeWorldMatrix() const;
		DirectX::XMMATRIX getInvTransposeWorldMatrix() const;
	public:
		GET_CLASS_NAME(Transform3D);
		GET_SET(DirectX::XMFLOAT3, translation);
		GET_SET(DirectX::XMFLOAT3, rotation);
		GET_SET(DirectX::XMFLOAT3, scale);
	public:
		bool operator==(const Transform& other);
		void moveZAxis(float distance);
		void moveXAxis(float distance);
		void rotateXAxis(float angle);
		void rotateYAxis(float angle);
	private:
		DirectX::XMFLOAT3 m_translation;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;
	};

	inline bool Transform::operator==(const Transform& other)
	{
		return 
			m_translation.x == other.m_translation.x &&
		    m_translation.y == other.m_translation.y &&
			m_translation.z == other.m_translation.z &&

			m_rotation.x == other.m_rotation.x &&
			m_rotation.y == other.m_rotation.y &&
			m_rotation.z == other.m_rotation.z &&

			m_scale.x == other.m_scale.x &&
			m_scale.y == other.m_scale.y &&
			m_scale.z == other.m_scale.z;
	}
}