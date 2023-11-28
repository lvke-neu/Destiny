#pragma once
#include "Visual3DComponent.h"
#include "Graphics/ConstantBuffer.h"
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;
	class BoxComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		BoxComponent();
	public:
		virtual void onAttachNode() override;
		virtual void onNodeTransformChanged() override;
	public:
		GET_CLASS_NAME(BoxComponent);
	private:
		void beforeDrawCommands();
	private:
		std::shared_ptr<ConstantBuffer<XMMATRIX>> m_worldMatrix;
	};
}