#pragma once
#include "Engine/EventSystem.h"
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;
namespace Destiny
{
	class ConstantBuffer;
	class Transform;
	class Camera
	{
	public:
		Camera(ID3D11Device* device, ID3D11DeviceContext* context);
		~Camera();
	private:
		void onResize(void* data);
		void onUpdate(void* data);
		void updateViewMat();
		void onMousePress(void* data);
		void onMouseRelease(void* data);
		void onMouseMove(void* data);
	private:
		std::unique_ptr<ConstantBuffer> m_constantBufferView;
		std::unique_ptr<ConstantBuffer> m_constantBufferProj;

		Transform* m_transform;
		ID3D11DeviceContext* m_context;
		float m_oldMousePosX;
		float m_oldMousePosY;
		float m_deltaMousePosX;
		float m_deltaMousePosY;
	};
}