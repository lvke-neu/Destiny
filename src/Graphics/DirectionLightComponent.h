#pragma once
#include "Engine/Component.h"
#include "Math/Color.h"
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
		GET(Color, color);
		void set_color(Color color);
		GET(float, intensity);
		void set_intensity(float intensity);
		GET(float, viewPortWidth);
		void set_viewPortWidth(float viewPortWidth);
		GET(float, viewPortHeight);
		void set_viewPortHeight(float viewPortHeight);
		GET(float, nearz);
		void set_nearz(float nearz);
		GET(float, farz);
		void set_farz(float farz);
		GET(float, lightDiscoefficient);
		void set_lightDiscoefficient(float lightDiscoefficient);
		GET(float, resolutionWidth);
		void set_resolutionWidth(float resolutionWidth);
		GET(float, resolutionHeight);
		void set_resolutionHeight(float resolutionHeight);
	private:
		void				traversal(std::shared_ptr<Node> node, std::vector<DirectionLight>& directionLights, bool ignoreSelf = false);
		void				notifyVisualRendererConstantChanged(std::shared_ptr<Node> node);
		void				updateDirectionLightRendererConstant(const std::vector<DirectionLight>& directionLights);
		void				updateShadowMapRendererConstant();
		void				notifyShadoMapPipiline();
	private:
		Color				m_color;
		float				m_intensity;

		//shadow
		float m_viewPortWidth;
		float m_viewPortHeight;
		float m_nearz;
		float m_farz;
		float m_lightDiscoefficient;
		float m_resolutionWidth;
		float m_resolutionHeight;
	};
}