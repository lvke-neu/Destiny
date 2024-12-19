#pragma once
#include "DrawCommand.h"
#include <memory>
#include <unordered_set>

namespace Destiny
{
	class Component;
	class DrawParameters;
	class RenderPass;
	class Mesh;
	class Visual : public DrawCommand
	{
	public:
		Visual();
	public:
		std::shared_ptr<RenderPass>			getRenderPass();
		void								setRenderPass(std::shared_ptr<RenderPass> renderPass);
		std::shared_ptr<Mesh>				getMesh();
		void								setMesh(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Component>			getComponent();
		void bindComponent(std::shared_ptr<Component> component);
	public:
		void updateDrawParameters();
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

	inline void Visual::bindComponent(std::shared_ptr<Component> component)
	{
		m_component = component;
	}
}