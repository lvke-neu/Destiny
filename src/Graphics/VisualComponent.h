#pragma once
#include "Visual.h"
#include "Engine/Component.h"
#include "Math/Color.h"
#include "GraphicsDefine.h"

namespace Destiny
{
	class VisualComponent : public Component, public std::enable_shared_from_this<VisualComponent>
	{
		RTTR_ENABLE(Component);
	public:
		VisualComponent();
		virtual ~VisualComponent() = default;
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
		void							reCompileShader();
		std::shared_ptr<Material>		get_material();
		void							set_material(std::shared_ptr<Material> material);
	public:
		virtual void				onAddToNode() override;
		virtual void				onNodeTransformChanged() override;
		virtual void				onEnterScene();

		void						onRendererConstantChanged();
	public:
		void						setRenderPass(std::shared_ptr<RenderPass> renderPass);
		void						setMesh(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Mesh>		getMesh();
		std::shared_ptr<Visual>		getVisual();

		void						setShadowRenderPass(std::shared_ptr<RenderPass> renderPass);
		void						setShadowMesh(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Visual>		getShadowVisual();

		template<typename T>
		void						setConstant(const char* name, T value);
		void						setShaderResource(const char* name, std::shared_ptr<Texture> texture);
		void						setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState);
	private:
		std::shared_ptr<Visual> m_visual;
		std::shared_ptr<Visual> m_shadowVisual;
	};

	inline void VisualComponent::setRenderPass(std::shared_ptr<RenderPass> renderPass)
	{
		if (m_visual)
		{
			m_visual->setRenderPass(renderPass);
		}	
	}

	inline void VisualComponent::setMesh(std::shared_ptr<Mesh> mesh)
	{
		if (m_visual)
		{
			m_visual->setMesh(mesh);
		}
	}

	inline std::shared_ptr<Mesh> VisualComponent::getMesh()
	{
		if (m_visual)
		{
			return m_visual->getMesh();
		}
		return nullptr;
	}

	inline std::shared_ptr<Visual> VisualComponent::getVisual()
	{
		return m_visual;
	}

	inline void VisualComponent::setShadowRenderPass(std::shared_ptr<RenderPass> renderPass)
	{
		if (m_shadowVisual)
		{
			m_shadowVisual->setRenderPass(renderPass);
		}
	}

	inline void VisualComponent::setShadowMesh(std::shared_ptr<Mesh> mesh)
	{
		if (m_shadowVisual)
		{
			m_shadowVisual->setMesh(mesh);
		}
	}

	inline std::shared_ptr<Visual> VisualComponent::getShadowVisual()
	{
		return m_shadowVisual;
	}

	template<typename T>
	inline void VisualComponent::setConstant(const char* name, T value)
	{
		if (m_visual)
		{
			m_visual->setConstant(name, value);
		}
	}

	inline void VisualComponent::setShaderResource(const char* name, std::shared_ptr<Texture> texture)
	{
		if (m_visual)
		{
			m_visual->setShaderResource(name, texture);
		}
	}

	inline void VisualComponent::setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState)
	{
		if (m_visual)
		{
			m_visual->setSamplerSate(name, samplerState);
		}
	}
}