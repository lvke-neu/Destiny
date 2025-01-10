#pragma once
#include "Engine/Component.h"
#include "Math/Color32.h"

namespace Destiny
{
	class DirectionLightComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		DirectionLightComponent();
		virtual ~DirectionLightComponent() = default;
	public:
		virtual void		onEnterScene() override;
		virtual void		onNodeTransformChanged() override;
	public:
		GET(Color32, color);
		void set_color(Color32 color);
		GET(float, intensity);
		void set_intensity(float intensity);
	private:
		void				traversalLightDirectionChanged(std::shared_ptr<Node> node);
		void				traversalLightColorChanged(std::shared_ptr<Node> node);
		void				traversalLightIntensityChanged(std::shared_ptr<Node> node);
	private:
		Color32				m_color;
		float				m_intensity;
	};
}