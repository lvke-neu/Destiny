#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color32.h"

namespace Destiny
{
	class PlaneComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		PlaneComponent();
		virtual ~PlaneComponent() = default;
	public:
		GET(Color32, color);
		void set_color(Color32 color);
	private:
		Color32 m_color;
	};
}