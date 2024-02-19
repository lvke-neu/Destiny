#pragma once
#include "BoxComponent.h"
#include "Graphics/ConstantBuffer.h"
#include <memory>
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;
	class PointLightComponent : public BoxComponent
	{
		RTTR_ENABLE(BoxComponent);
	public:
		PointLightComponent();
	public:
		virtual void onAttachNode() override;
		virtual void onNodeTransformChanged() override;
	public:
		GET_CLASS_NAME(PointLightComponent);
	private:
		struct cbPointLight
		{
			XMFLOAT4 position;
			XMFLOAT4 color;
		};
		std::shared_ptr<ConstantBuffer<cbPointLight>> m_direction;
	};
}