#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class SkyboxComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		SkyboxComponent();
		virtual ~SkyboxComponent() = default;
	public:
		GET(std::string, texture);
		void set_texture(std::string texture);
	private:
		std::string m_texture;
	};
}