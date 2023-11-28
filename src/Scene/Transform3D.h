#pragma once
#include "Engine/Reflection.h"
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;
	class Transform3D : public Reflection
	{
		RTTR_ENABLE(Reflection);
	public:
		Transform3D();
	public:
		XMMATRIX getWorldMatrix();
		void moveZAxis(float distance);
		void moveXAxis(float distance);
		void rotateXAxis(float angle);
		void rotateYAxis(float angle);
	public:
		GET_CLASS_NAME(Transform3D);
		GET_SET(XMFLOAT3, translation);
		GET_SET(XMFLOAT3, rotation);
		GET_SET(XMFLOAT3, scale);
	private:
		XMFLOAT3 m_translation;
		XMFLOAT3 m_rotation;
		XMFLOAT3 m_scale;
	};
}