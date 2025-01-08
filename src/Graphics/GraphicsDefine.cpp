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

	rttr::registration::enumeration<D3D11_DEPTH_WRITE_MASK>("D3D11_DEPTH_WRITE_MASK")
		(
			rttr::value("D3D11_DEPTH_WRITE_MASK_ZERO", D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO),
			rttr::value("D3D11_DEPTH_WRITE_MASK_ALL", D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL)
		);

	rttr::registration::enumeration<D3D11_COMPARISON_FUNC>("D3D11_COMPARISON_FUNC")
		(
			rttr::value("D3D11_COMPARISON_NEVER", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER),
			rttr::value("D3D11_COMPARISON_LESS", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS),
			rttr::value("D3D11_COMPARISON_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL),
			rttr::value("D3D11_COMPARISON_LESS_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL),
			rttr::value("D3D11_COMPARISON_GREATER", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER),
			rttr::value("D3D11_COMPARISON_NOT_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL),
			rttr::value("D3D11_COMPARISON_GREATER_EQUAL", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER_EQUAL),
			rttr::value("D3D11_COMPARISON_ALWAYS", D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS)
		);

	rttr::registration::enumeration<D3D11_STENCIL_OP>("D3D11_STENCIL_OP")
		(
			rttr::value("D3D11_STENCIL_OP_KEEP", D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP),
			rttr::value("D3D11_STENCIL_OP_ZERO", D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO),
			rttr::value("D3D11_STENCIL_OP_REPLACE", D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE),
			rttr::value("D3D11_STENCIL_OP_INCR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT),
			rttr::value("D3D11_STENCIL_OP_DECR_SAT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT),
			rttr::value("D3D11_STENCIL_OP_INVERT", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT),
			rttr::value("D3D11_STENCIL_OP_INCR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR),
			rttr::value("D3D11_STENCIL_OP_DECR", D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR)
		);

	rttr::registration::class_<D3D11_DEPTH_STENCILOP_DESC>("D3D11_DEPTH_STENCILOP_DESC")
		.constructor<>()
		.property("StencilFailOp", &D3D11_DEPTH_STENCILOP_DESC::StencilFailOp)
		.property("StencilDepthFailOp", &D3D11_DEPTH_STENCILOP_DESC::StencilDepthFailOp)
		.property("StencilPassOp", &D3D11_DEPTH_STENCILOP_DESC::StencilPassOp)
		.property("StencilFunc", &D3D11_DEPTH_STENCILOP_DESC::StencilFunc);

	rttr::registration::class_<D3D11_DEPTH_STENCIL_DESC>("D3D11_DEPTH_STENCIL_DESC")
		.constructor<>()
		.property("DepthEnable", &D3D11_DEPTH_STENCIL_DESC::DepthEnable)
		.property("DepthWriteMask", &D3D11_DEPTH_STENCIL_DESC::DepthWriteMask)
		.property("DepthFunc", &D3D11_DEPTH_STENCIL_DESC::DepthFunc)
		.property("StencilEnable", &D3D11_DEPTH_STENCIL_DESC::StencilEnable)
		.property("StencilReadMask", &D3D11_DEPTH_STENCIL_DESC::StencilReadMask)
		.property("StencilWriteMask", &D3D11_DEPTH_STENCIL_DESC::StencilWriteMask)
		.property("FrontFace", &D3D11_DEPTH_STENCIL_DESC::FrontFace)
		.property("BackFace", &D3D11_DEPTH_STENCIL_DESC::BackFace);

	rttr::registration::enumeration<D3D11_BLEND>("D3D11_BLEND")
		(
			rttr::value("D3D11_BLEND_ZERO", D3D11_BLEND::D3D11_BLEND_ZERO),
			rttr::value("D3D11_BLEND_ONE", D3D11_BLEND::D3D11_BLEND_ONE),
			rttr::value("D3D11_BLEND_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_SRC_COLOR),
			rttr::value("D3D11_BLEND_INV_SRC_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR),
			rttr::value("D3D11_BLEND_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA),
			rttr::value("D3D11_BLEND_INV_SRC_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA),
			rttr::value("D3D11_BLEND_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_DEST_ALPHA),
			rttr::value("D3D11_BLEND_INV_DEST_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA),
			rttr::value("D3D11_BLEND_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_DEST_COLOR),
			rttr::value("D3D11_BLEND_INV_DEST_COLOR", D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR),
			rttr::value("D3D11_BLEND_SRC_ALPHA_SAT", D3D11_BLEND::D3D11_BLEND_SRC_ALPHA_SAT),
			rttr::value("D3D11_BLEND_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR),
			rttr::value("D3D11_BLEND_INV_BLEND_FACTOR", D3D11_BLEND::D3D11_BLEND_INV_BLEND_FACTOR),
			rttr::value("D3D11_BLEND_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_SRC1_COLOR),
			rttr::value("D3D11_BLEND_INV_SRC1_COLOR", D3D11_BLEND::D3D11_BLEND_INV_SRC1_COLOR),
			rttr::value("D3D11_BLEND_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_SRC1_ALPHA),
			rttr::value("D3D11_BLEND_INV_SRC1_ALPHA", D3D11_BLEND::D3D11_BLEND_INV_SRC1_ALPHA)
		);
	rttr::registration::enumeration<D3D11_BLEND_OP>("D3D11_BLEND_OP")
		(
			rttr::value("D3D11_BLEND_OP_ADD", D3D11_BLEND_OP::D3D11_BLEND_OP_ADD),
			rttr::value("D3D11_BLEND_OP_SUBTRACT", D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT),
			rttr::value("D3D11_BLEND_OP_REV_SUBTRACT", D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT),
			rttr::value("D3D11_BLEND_OP_MIN", D3D11_BLEND_OP::D3D11_BLEND_OP_MIN),
			rttr::value("D3D11_BLEND_OP_MAX", D3D11_BLEND_OP::D3D11_BLEND_OP_MAX)
		);
	rttr::registration::class_<D3D11_RENDER_TARGET_BLEND_DESC>("D3D11_RENDER_TARGET_BLEND_DESC")
		.constructor<>()
		.property("BlendEnable", &D3D11_RENDER_TARGET_BLEND_DESC::BlendEnable)
		.property("SrcBlend", &D3D11_RENDER_TARGET_BLEND_DESC::SrcBlend)
		.property("DestBlend", &D3D11_RENDER_TARGET_BLEND_DESC::DestBlend)
		.property("BlendOp", &D3D11_RENDER_TARGET_BLEND_DESC::BlendOp)
		.property("SrcBlendAlpha", &D3D11_RENDER_TARGET_BLEND_DESC::SrcBlendAlpha)
		.property("DestBlendAlpha", &D3D11_RENDER_TARGET_BLEND_DESC::DestBlendAlpha)
		.property("BlendOpAlpha", &D3D11_RENDER_TARGET_BLEND_DESC::BlendOpAlpha)
		.property("RenderTargetWriteMask", &D3D11_RENDER_TARGET_BLEND_DESC::RenderTargetWriteMask);
	rttr::registration::class_<D3D11_BLEND_DESC>("D3D11_BLEND_DESC")
		.constructor<>()
		.property("AlphaToCoverageEnable", &D3D11_BLEND_DESC::AlphaToCoverageEnable)
		.property("IndependentBlendEnable", &D3D11_BLEND_DESC::IndependentBlendEnable)
		.property("RenderTarget", &D3D11_BLEND_DESC::RenderTarget);

	}

}