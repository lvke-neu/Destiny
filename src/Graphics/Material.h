#pragma once
#include <memory>

namespace Destiny
{
	class Renderer;
	class Material
	{
	public:
		Material();
		virtual ~Material() = default;
	public:
		void bind(std::shared_ptr<Renderer> renderer);
	private:
		virtual void update() = 0;
	protected:
		std::shared_ptr<Renderer> m_renderer;
	};
}