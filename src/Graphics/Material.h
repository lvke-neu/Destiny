#pragma once
#include "Engine/Object.h"
#include <memory>

namespace Destiny
{
	class Renderer;
	class Material : public Object
	{
		RTTR_ENABLE(Object);
	public:
		Material();
		virtual ~Material() = default;
	public:
		void bind(std::shared_ptr<Renderer> renderer);
	private:
		virtual void update(){};
	protected:
		std::shared_ptr<Renderer> m_renderer;
	};
}