#pragma once
#include <memory>
#include <unordered_set>

namespace Destiny
{
	class Effect;
	class Mesh;
	class DrawCommand;
	class Visual
	{
	public:
		Visual();
	public:
		std::shared_ptr<Effect> getEffect();
		void setEffect(std::shared_ptr<Effect> effect);
		std::shared_ptr<Mesh> getMesh();
		void setMesh(std::shared_ptr<Mesh> mesh);
		void fillDrawCommand(std::shared_ptr<DrawCommand> drawCommand);
	private:
		std::shared_ptr<Effect> m_effect;
		std::shared_ptr<Mesh> m_mesh;
	};

	inline std::shared_ptr<Effect> Visual::getEffect()
	{
		return m_effect;
	}

	inline std::shared_ptr<Mesh> Visual::getMesh()
	{
		return m_mesh;
	}
}