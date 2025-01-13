#pragma once
#include "Engine/Component.h"
#include "Math/Color32.h"
#include "Graphics/GraphicsDefine.h"

namespace Destiny
{
	class DirectionLightComponent : public Component, public std::enable_shared_from_this<DirectionLightComponent>
	{
		RTTR_ENABLE(Component);
	public:
		DirectionLightComponent();
		virtual ~DirectionLightComponent() = default;
	public:
		virtual void		onEnterScene() override;
		virtual void		onLeaveScene() override;
		virtual void		onNodeTransformChanged() override;
		virtual void		onPropertyChanged(const std::string& property) override;
	public:
		GET(Color32, color);
		void set_color(Color32 color);
		GET(float, intensity);
		void set_intensity(float intensity);
	private:
		void				traversal(std::shared_ptr<Node> node, std::vector<DirectionLight>& directionLights, bool ignoreSelf = false);
		void				traversalDirectionLightChanged(std::shared_ptr<Node> node, const std::vector<DirectionLight>& directionLights);
	private:
		Color32				m_color;
		float				m_intensity;
	};
}