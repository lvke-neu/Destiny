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
		virtual void				onNodeTransformChanged() override;
	public:
		GET(std::string, texture);
		void set_texture(std::string texture);

		GET(float, exposure);
		void set_exposure(float exposure);
	private:
		std::string m_texture;
		float m_exposure;
	};
}