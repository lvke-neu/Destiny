#include "PostProcessPipeline.h"
#include "BindRenderTargets.h"
#include "Visual.h"
#include "Renderer.h"
#include "RenderStates.h"
#include "RenderPass.h"
#include "InputLayout.h"
#include "Mesh.h"
#include "MeshProvider.h"
#include "SamplerState.h"
#include "Texture.h"

namespace Destiny
{
	PostProcessPipeline::PostProcessPipeline() :
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_fullScreenTriangle(std::make_shared<Visual>())
	{
		auto sampler = std::make_shared<SamplerState>();
		sampler->load(0);
		auto texture = Texture::Create("builtin://texture/box_diffuse.png");
		texture->load(0);

		auto renderer = std::make_shared<Renderer>("builtin://renderer/full_screen_triangle.hlsl");
		renderer->load(0);
		renderer->setSamplerSate("s_sampler", sampler);
		renderer->setShaderResource("t_texture", texture);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getDepthStencilStateDesc()->DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		renderStates->load(0);

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_FullScreenTriangle();
		mesh->load(0);

		m_fullScreenTriangle->setRenderPass(renderPass);
		m_fullScreenTriangle->setMesh(mesh);
	}

	PostProcessPipeline::~PostProcessPipeline()
	{

	}

	void PostProcessPipeline::syncState()
	{
		GraphicsPipeline::syncState();
		addGraphicsCommand(m_bindRenderTargets);
		m_fullScreenTriangle->updateDrawParameters();
		addGraphicsCommand(m_fullScreenTriangle);
	}

	std::shared_ptr<RenderTargetView> PostProcessPipeline::getRTV()
	{
		if (!m_bindRenderTargets)
		{
			return nullptr;
		}

		return m_bindRenderTargets->getRenderTargetView();
	}
}