#pragma once
#include "Engine/Object.h"
#include <memory>

namespace Destiny
{
	class Visual;
	class Material : public Object
	{
		RTTR_ENABLE(Object);
	public:
		Material();
		virtual ~Material() = default;
	public:
		void bind(std::shared_ptr<Visual> visual);
	private:
		virtual void update(){};
	protected:
		std::shared_ptr<Visual> m_visual;
	};
}