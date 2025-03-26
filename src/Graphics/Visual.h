#pragma once
#include "DrawCommand.h"
#include "RenderPass.h"

namespace Destiny
{
	class Component;
	class Mesh;
	class Visual : public DrawCommand, public std::enable_shared_from_this<Visual>
	{
	public:
		Visual();
	public:
		//for ui 
		std::string get_renderer();
		void set_renderer(std::string renderer);
		RendererCategory				get_rendererCategory();
		void							set_rendererCategory(RendererCategory rendererCategory);
		D3D11_RASTERIZER_DESC			get_rasterizerDesc();
		void							set_rasterizerDesc(D3D11_RASTERIZER_DESC desc);
		D3D11_DEPTH_STENCIL_DESC		get_depthStencilDesc();
		void							set_depthStencilDesc(D3D11_DEPTH_STENCIL_DESC desc);
		D3D11_BLEND_DESC				get_blendDesc();
		void							set_blendDesc(D3D11_BLEND_DESC desc);
		std::shared_ptr<Material>		get_material();
		void							set_material(std::shared_ptr<Material> material);
	public:
		std::shared_ptr<RenderPass>			getRenderPass();
		void								setRenderPass(std::shared_ptr<RenderPass> renderPass);
		std::shared_ptr<Mesh>				getMesh();
		void								setMesh(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Component>			getComponent();
		void								setComponent(std::shared_ptr<Component> component);
	public:
		void								updateDrawParameters();
		void								load(int priority = 1);

		template<typename T>
		void								setConstant(const char* name, T value);
		void								setConstant(const char* name, std::shared_ptr<Blob> blob);
		void								setRendererConstantChanged();
		void								setShaderResource(const char* name, std::shared_ptr<Texture> texture);
		void								setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState);
		void							    reCompileShader();
	private:
		std::shared_ptr<RenderPass>			m_renderPass;
		std::shared_ptr<Mesh>				m_mesh;
		std::shared_ptr<DrawParameters>		m_drawParameters;
		std::shared_ptr<Component>			m_component;
		std::shared_ptr<Material>			m_material;

		std::unordered_map<std::string, std::shared_ptr<Blob>> m_constants;
		std::unordered_map<std::string, std::string> m_variableLinkConstant;
		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>> m_constantBuffers;

		std::unordered_map<std::string, std::shared_ptr<Texture>> m_visualTextures;
		std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>	m_textures;

		std::unordered_map<std::string, std::shared_ptr<SamplerState>> m_visualSamplerStates;
		std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>> m_samplerStates;

		bool m_constantsChanged;
		bool m_rendererConstantsChanged;
		bool m_texturesChanged;
		bool m_samplerStatesrChanged;
		bool m_renderPassChanged;
		bool m_meshChanged;
	};
	
	inline std::shared_ptr<RenderPass> Visual::getRenderPass()
	{
		return m_renderPass;
	}

	inline std::shared_ptr<Mesh> Visual::getMesh()
	{
		return m_mesh;
	}

	inline std::shared_ptr<Component> Visual::getComponent()
	{
		return m_component;
	}

	inline void Visual::setComponent(std::shared_ptr<Component> component)
	{
		m_component = component;
	}

	template<typename T>
	inline void Visual::setConstant(const char* name, T value)
	{
		std::shared_ptr<Blob> blob = std::make_shared<Blob>(sizeof(value));
		blob->copyfrom(&value, sizeof(value));
		m_constants[name] = blob;
		m_constantsChanged = true;
	}

	inline void Visual::setConstant(const char* name, std::shared_ptr<Blob> blob)
	{
		m_constants[name] = blob;
		m_constantsChanged = true;
	}


	inline void Visual::setRendererConstantChanged()
	{
		m_rendererConstantsChanged = true;
	}

	inline void Visual::reCompileShader()
	{
		if (m_renderPass)
		{
			m_renderPass->reCompileShader();
		}
	}
}