#include "GraphicsDefine.h"
#include<rttr/registration.h>
#include <d3d11.h>

namespace Destiny
{
	RTTR_REGISTRATION
	{
	rttr::registration::enumeration<RendererCategory>("RendererCategory")
		(
			rttr::value("None", RendererCategory::None),
			rttr::value("ForwardOpaque", RendererCategory::ForwardOpaque),
			rttr::value("ForwardTransparent", RendererCategory::ForwardTransparent),
			rttr::value("Gui", RendererCategory::Gui)
		);

	rttr::registration::enumeration<D3D11_FILL_MODE>("D3D11_FILL_MODE")
		(
			rttr::value("D3D11_FILL_WIREFRAME", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME),
			rttr::value("D3D11_FILL_SOLID", D3D11_FILL_MODE::D3D11_FILL_SOLID)
		);

	rttr::registration::enumeration<D3D11_CULL_MODE>("D3D11_CULL_MODE")
		(
			rttr::value("D3D11_CULL_NONE", D3D11_CULL_MODE::D3D11_CULL_NONE),
			rttr::value("D3D11_CULL_FRONT", D3D11_CULL_MODE::D3D11_CULL_FRONT),
			rttr::value("D3D11_CULL_BACK", D3D11_CULL_MODE::D3D11_CULL_BACK)
		);

	rttr::registration::class_<D3D11_RASTERIZER_DESC>("D3D11_RASTERIZER_DESC")
		.constructor<>()
		.property("FillMode", &D3D11_RASTERIZER_DESC::FillMode)
		.property("CullMode", &D3D11_RASTERIZER_DESC::CullMode)
		.property("FrontCounterClockwise", &D3D11_RASTERIZER_DESC::FrontCounterClockwise)
		.property("DepthBias", &D3D11_RASTERIZER_DESC::DepthBias)
		.property("DepthBiasClamp", &D3D11_RASTERIZER_DESC::DepthBiasClamp)
		.property("SlopeScaledDepthBias", &D3D11_RASTERIZER_DESC::SlopeScaledDepthBias)
		.property("DepthClipEnable", &D3D11_RASTERIZER_DESC::DepthClipEnable)
		.property("ScissorEnable", &D3D11_RASTERIZER_DESC::ScissorEnable)
		.property("MultisampleEnable", &D3D11_RASTERIZER_DESC::MultisampleEnable)
		.property("AntialiasedLineEnable", &D3D11_RASTERIZER_DESC::AntialiasedLineEnable);
	}
}