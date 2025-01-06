#pragma once
#include "DrawCommand.h"
#include "RenderPass.h"

namespace Destiny
{
	class Component;
	class Mesh;
	class Visual : public DrawCommand
	{
	public:
		Visual();
	public:
		//for ui 
		std::string get_renderer();
		void set_renderer(std::string renderer);
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
		void								setShaderResource(const char* name, std::shared_ptr<Texture> texture);
		void								setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState);
	private:
		std::shared_ptr<RenderPass>			m_renderPass;
		std::shared_ptr<Mesh>				m_mesh;
		std::shared_ptr<DrawParameters>		m_drawParameters;
		std::shared_ptr<Component>			m_component;
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
		if (m_renderPass)
		{
			m_renderPass->setConstant(name, value);
		}
	}

	inline void Visual::setShaderResource(const char* name, std::shared_ptr<Texture> texture)
	{
		if (m_renderPass)
		{
			m_renderPass->setShaderResource(name, texture);
		}
	}

	inline void Visual::setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState)
	{
		if (m_renderPass)
		{
			m_renderPass->setSamplerSate(name, samplerState);
		}
	}
}