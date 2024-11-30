#pragma once
#include "DrawCommand.h"
#include <memory>
#include <unordered_set>

namespace Destiny
{
	class DrawParameters;
	class RenderPass;
	class Mesh;
	class Visual : public DrawCommand
	{
	public:
		Visual();
	public:
		std::shared_ptr<RenderPass> getRenderPass();
		void setRenderPass(std::shared_ptr<RenderPass> renderPass);
		std::shared_ptr<Mesh> getMesh();
		void setMesh(std::shared_ptr<Mesh> mesh);
	public:
		void upDrawParameters();
	private:
		std::shared_ptr<RenderPass> m_renderPass;
		std::shared_ptr<Mesh> m_mesh;
		std::shared_ptr<DrawParameters> m_drawParameters;
	};
	
	inline std::shared_ptr<RenderPass> Visual::getRenderPass()
	{
		return m_renderPass;
	}

	inline std::shared_ptr<Mesh> Visual::getMesh()
	{
		return m_mesh;
	}
}