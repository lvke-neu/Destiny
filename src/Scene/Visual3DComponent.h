#pragma once
#include "Component.h"
#include "Graphics/RasterizerState.h"

namespace Destiny
{
	class Visual3D;
	class Visual3DComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		Visual3DComponent();
	public:
		GET_CLASS_NAME(Visual3DComponent);
		GET(std::shared_ptr<Visual3D>, visual3D);
		GET(D3D11_RASTERIZER_DESC, rasterizerStateDesc);

		void set_rasterizerStateDesc(D3D11_RASTERIZER_DESC rasterizerStateDesc);
	protected:
		std::shared_ptr<Visual3D> m_visual3D;
		D3D11_RASTERIZER_DESC m_rasterizerStateDesc;
	};
}