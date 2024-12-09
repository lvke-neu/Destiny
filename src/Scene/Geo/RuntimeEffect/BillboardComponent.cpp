#include "BillboardComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include <d3d11.h>

namespace Destiny
{
	BillboardComponent::BillboardComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/billboard.hlsl");
		renderer->load(0);
		renderer->setConstant("c_size", DirectX::XMFLOAT2{ 150.0f, 150.0f });

		auto texture = Texture::Create("builtin://texture/tree0.dds");
		texture->load();
		renderer->setShaderResource("t_texture", texture);

		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load(0);
		renderer->setSamplerSate("s_sampler", samplerState);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getBlendStateDesc()->AlphaToCoverageEnable = true;
		renderStates->getBlendStateDesc()->IndependentBlendEnable = false;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendEnable = true;
		renderStates->getBlendStateDesc()->RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderStates->getBlendStateDesc()->RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		renderStates->getBlendStateDesc()->RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		renderStates->getBlendStateDesc()->RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		renderStates->getBlendStateDesc()->RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		renderStates->getDepthStencilStateDesc()->DepthEnable = false;
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RenderPass::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Position();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BillboardComponent>("BillboardComponent")
			.constructor<>();
	}
}