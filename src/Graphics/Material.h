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
		virtual ~Material();
	public:
		void bind(std::shared_ptr<Visual> visual);
	private:
		virtual void update(){};
	protected:
		std::weak_ptr<Visual> m_visual;
	};
}