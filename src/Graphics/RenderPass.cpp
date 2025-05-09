#include "RenderPass.h"
#include "RenderStates.h"

namespace Destiny
{
	RenderPass::RenderPass() :
		m_renderer(nullptr),
		m_renderStates(std::make_shared<RenderStates>()),
		m_rendererCategory(RendererCategory::None)
	{

	}

	std::string RenderPass::get_renderer()
	{
		return m_renderer ? m_renderer->getPath() : "";
	}

	void RenderPass::set_renderer(std::string renderer)
	{
		auto rdr = Renderer::Create(renderer.c_str());
		rdr->load(0);
		if (rdr->isLoadingSucceed())
		{
			setRenderer(rdr);
		}
	}

	D3D11_RASTERIZER_DESC RenderPass::get_rasterizerDesc()
	{
		if (m_renderStates)
		{
			return *m_renderStates->getRasterizerStateDesc();
		}
		return D3D11_RASTERIZER_DESC();
	}

	void RenderPass::set_rasterizerDesc(D3D11_RASTERIZER_DESC desc)
	{
		if (m_renderStates && m_renderStates->getRasterizerStateDesc())
		{
			memcpy_s(m_renderStates->getRasterizerStateDesc().get(), sizeof(D3D11_RASTERIZER_DESC), &desc, sizeof(D3D11_RASTERIZER_DESC));
			m_renderStates->load(0);
		}
	}

	D3D11_DEPTH_STENCIL_DESC RenderPass::get_depthStencilDesc()
	{
		if (m_renderStates)
		{
			return *m_renderStates->getDepthStencilStateDesc();
		}
		return D3D11_DEPTH_STENCIL_DESC();
	}

	void RenderPass::set_depthStencilDesc(D3D11_DEPTH_STENCIL_DESC desc)
	{
		if (m_renderStates && m_renderStates->getDepthStencilStateDesc())
		{
			memcpy_s(m_renderStates->getDepthStencilStateDesc().get(), sizeof(D3D11_DEPTH_STENCIL_DESC), &desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
			m_renderStates->load(0);
		}
	}

	D3D11_BLEND_DESC RenderPass::get_blendDesc()
	{
		if (m_renderStates)
		{
			return *m_renderStates->getBlendStateDesc();
		}
		return D3D11_BLEND_DESC();
	}

	void RenderPass::set_blendDesc(D3D11_BLEND_DESC desc)
	{
		if (m_renderStates && m_renderStates->getBlendStateDesc())
		{
			memcpy_s(m_renderStates->getBlendStateDesc().get(), sizeof(D3D11_BLEND_DESC), &desc, sizeof(D3D11_BLEND_DESC));
			m_renderStates->load(0);
		}
	}

	void RenderPass::setRenderer(std::shared_ptr<Renderer> renderer)
	{
		//if (!renderer)
		//{
		//	return;
		//}

		//if (renderer->isLoadingPending())
		//{
		//	renderer->load(0);
		//}

		//if (!renderer->isLoadingSucceed())
		//{
		//	return;
		//}
		if (!renderer)
		{
			return;
		}

		//renderer->copy_constant_texture_sampler(m_renderer);
		m_renderer.reset();
		m_renderer = renderer;
	}

	void RenderPass::setRenderStates(std::shared_ptr<RenderStates> renderStates)
	{
		if (!renderStates)
		{
			return;
		}

		if (renderStates->isLoadingPending())
		{
			renderStates->load(0);
		}

		if (!renderStates->isLoadingSucceed())
		{
			return;
		}

		m_renderStates.reset();
		m_renderStates = renderStates;
	}

	void RenderPass::fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters)
	{
		if (m_renderer)
		{
			m_renderer->fillDrawParameters(drawParameters);
		}

		if (m_renderStates)
		{
			m_renderStates->fillDrawParameters(drawParameters);
		}
	}

	void RenderPass::fillConstantBuffers(std::unordered_map<std::string, std::string>& variableLinkConstant, std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& constantBuffers)
	{
		if (m_renderer)
		{
			m_renderer->fillConstantBuffers(variableLinkConstant, constantBuffers);
		}
	}

	void RenderPass::modifyConstantBuffersByDifference(std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& constantBuffers)
	{
		if (m_renderer)
		{
			m_renderer->modifyConstantBuffersByDifference(constantBuffers);
		}
	}

	void RenderPass::fillTextures(std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>& textures)
	{
		if (m_renderer)
		{
			m_renderer->fillTextures(textures);
		}
	}

	void RenderPass::modifyTexturesByDifferenece(std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>& textures)
	{
		if (m_renderer)
		{
			m_renderer->modifyTexturesByDifferenece(textures);
		}
	}

	void RenderPass::fillSamplerStates(std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>& samplerStates)
	{
		if (m_renderer)
		{
			m_renderer->fillSamplerStates(samplerStates);
		}
	}

	void RenderPass::modifySamplerStatesByDifferenece(std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>& samplerStates)
	{
		if (m_renderer)
		{
			m_renderer->modifySamplerStatesByDifferenece(samplerStates);
		}
	}

	void RenderPass::load(int priority)
	{
		if (m_renderer)
		{
			m_renderer->load(priority);
		}
		if (m_renderStates)
		{
			m_renderStates->load(priority);
		}
	}
}
