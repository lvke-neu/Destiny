#pragma once
#include "Component.h"
#include "Graphics/ConstantBuffer.h"
#include <memory>
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;
	class DirectLightComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		DirectLightComponent();
	public:
		virtual void onAttachNode() override;
		virtual void onNodeTransformChanged() override;
	public:
		GET_CLASS_NAME(DirectLightComponent);
	private:
		std::shared_ptr<ConstantBuffer<XMFLOAT4>> m_direction;
	};
}