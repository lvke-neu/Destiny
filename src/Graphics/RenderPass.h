#pragma once
#include "Renderer.h"
#include "GraphicsDefine.h"
#include <d3d11.h>

namespace Destiny
{
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
	public:
		std::shared_ptr<Renderer>		getRenderer();
		void							setRenderer(std::shared_ptr<Renderer> renderer);

		std::shared_ptr<RenderStates>	getRenderStates();
		void							setRenderStates(std::shared_ptr<RenderStates> renderStates);
		
		RendererCategory				getRendererCategory();
		void							setRendererCategory(RendererCategory rendererCategory);

		void							fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters);
		void							load(int priority = 1);
		template<typename T>
		void							setConstant(const char* name, T value);
		void							setShaderResource(const char* name, std::shared_ptr<Texture> texture);
		void							setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState);
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

	template<typename T>
	inline void RenderPass::setConstant(const char* name, T value)
	{
		if (m_renderer)
		{
			m_renderer->setConstant(name, value);
		}
	}

	inline void RenderPass::setShaderResource(const char* name, std::shared_ptr<Texture> texture)
	{
		if (m_renderer)
		{
			m_renderer->setShaderResource(name, texture);
		}
	}

	inline void RenderPass::setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState)
	{
		if (m_renderer)
		{
			m_renderer->setSamplerSate(name, samplerState);
		}
	}
}