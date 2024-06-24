#pragma once
#include <memory>
#include <unordered_set>

namespace Destiny
{
	class Effect;
	class Mesh;
	class RenderParameters;
	class Visual3D
	{
	public:
		Visual3D();
	public:
		std::shared_ptr<Effect> getEffect();
		void setEffect(std::shared_ptr<Effect> effect);
		std::shared_ptr<Mesh> getMesh();
		void setMesh(std::shared_ptr<Mesh> mesh);
	public:
		std::unordered_set<std::shared_ptr<RenderParameters>> getRenderParameters();
	private:
		void createRenderParameters();
	private:
		std::shared_ptr<Effect> m_effect;
		std::shared_ptr<Mesh> m_mesh;
		std::unordered_set<std::shared_ptr<RenderParameters>> m_renderParameters;
	};

	inline std::shared_ptr<Effect> Visual3D::getEffect()
	{
		return m_effect;
	}

	inline std::shared_ptr<Mesh> Visual3D::getMesh()
	{
		return m_mesh;
	}
}