#pragma once
#include "Renderer.h"
#include "Material.h"
#include "GraphicsDefine.h"
#include <d3d11.h>

namespace Destiny
{
	class Material;
	class RenderStates;
	class DrawParameters;
	class Texture;
	class SamplerState;
	class RenderPass
	{
	public:
		RenderPass();
	public:
		//for ui 
		std::string get_renderer();
		void set_renderer(std::string renderer);
		D3D11_RASTERIZER_DESC			get_rasterizerDesc();
		void							set_rasterizerDesc(D3D11_RASTERIZER_DESC desc);
		D3D11_DEPTH_STENCIL_DESC		get_depthStencilDesc();
		void							set_depthStencilDesc(D3D11_DEPTH_STENCIL_DESC desc);
		D3D11_BLEND_DESC				get_blendDesc();
		void							set_blendDesc(D3D11_BLEND_DESC desc);
	public:
		std::shared_ptr<Renderer>		getRenderer();
		void							setRenderer(std::shared_ptr<Renderer> renderer);

		std::shared_ptr<RenderStates>	getRenderStates();
		void							setRenderStates(std::shared_ptr<RenderStates> renderStates);
		
		RendererCategory				getRendererCategory();
		void							setRendererCategory(RendererCategory rendererCategory);

		void							fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters);
		void							fillConstantBuffers(std::unordered_map<std::string, std::string>& variableLinkConstant, std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& constantBuffers);
		void							fillTextures(std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>& textures);
		void							fillSamplerStates(std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>& samplerStates);

		void							load(int priority = 1);

		void							reCompileShader();
	private:
		std::shared_ptr<Renderer>		m_renderer;
		std::shared_ptr<RenderStates>	m_renderStates;
		RendererCategory m_rendererCategory;
	};

	inline std::shared_ptr<Renderer> RenderPass::getRenderer()
	{
		return m_renderer;
	}

	inline std::shared_ptr<RenderStates> RenderPass::getRenderStates()
	{
		return m_renderStates;
	}

	inline RendererCategory RenderPass::getRendererCategory()
	{
		return m_rendererCategory;
	}

	inline void RenderPass::setRendererCategory(RendererCategory rendererCategory)
	{
		m_rendererCategory = rendererCategory;
	}

	inline void RenderPass::reCompileShader()
	{
		if (m_renderer)
		{
			m_renderer->reload(0);
		}
	}
}