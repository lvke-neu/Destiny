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
		Transform(const Transform& other);
	public:
		DirectX::XMMATRIX		getWorldMatrix() const;
		void					setWorldMatrix(const DirectX::XMMATRIX& worldMatrix);
		DirectX::XMMATRIX		getTransposeWorldMatrix() const;
		DirectX::XMMATRIX		getInverseWorldMatrix() const;
		DirectX::XMMATRIX		getInvTransposeWorldMatrix() const;
	public:
		GET(DirectX::XMFLOAT3, translation);
		void set_translation(DirectX::XMFLOAT3 translation);
		GET(DirectX::XMFLOAT3, rotation);
		void set_rotation(DirectX::XMFLOAT3 rotation);
		GET(DirectX::XMFLOAT3, scale);
		void set_scale(DirectX::XMFLOAT3 scale);
	public:
		void					moveZAxis(float distance);
		void					moveXAxis(float distance);
		void					rotateXAxis(float angle);
		void					rotateYAxis(float angle);
	private:
		void					updateWorldMatrix();
	private:
		DirectX::XMFLOAT3		m_translation;
		DirectX::XMFLOAT3		m_rotation;
		DirectX::XMFLOAT3		m_scale;
		DirectX::XMMATRIX		m_worldMatrix;
	};
}