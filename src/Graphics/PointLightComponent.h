#pragma once
#include "Engine/Component.h"
#include "Math/Color.h"
#include "Graphics/GraphicsDefine.h"

namespace Destiny
{
	class PointLightComponent : public Component, public std::enable_shared_from_this<PointLightComponent>
	{
		RTTR_ENABLE(Component);
	public:
		PointLightComponent();
		virtual ~PointLightComponent() = default;
	public:
		virtual void		onEnterScene() override;
		virtual void		onLeaveScene() override;
		virtual void		onNodeTransformChanged() override;
		virtual void		onPropertyChanged(const std::string& property) override;
	public:
		GET(Color, color);
		void set_color(Color color);
		GET(float, intensity);
		void set_intensity(float intensity);
	private:
		void				traversal(std::shared_ptr<Node> node, std::vector<PointLight>& pointLights, bool ignoreSelf = false);
		void                notifyVisualRendererConstantChanged(std::shared_ptr<Node> node);
		void				setPointLightRendererConstant(const std::vector<PointLight>& pointLights);
	private:
		Color				m_color;
		float				m_intensity;
	};
}