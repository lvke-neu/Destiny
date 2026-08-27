#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class TextComponent;
	class SceneStatisticsComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		SceneStatisticsComponent();
		~SceneStatisticsComponent();
	public:
		virtual void onAddToNode() override;
		virtual void onEnterScene() override;
		virtual void onLeaveScene() override;
		virtual void onUpdate(float deltaTime) override;
	private:
		std::shared_ptr<TextComponent> m_frameTextComponent;
		std::shared_ptr<TextComponent> m_fpsTextComponent;
		std::shared_ptr<TextComponent> m_drawcallCountTextComponent;
		std::shared_ptr<TextComponent> m_triangleCountTextComponent;
		std::shared_ptr<TextComponent> m_visualCountTextComponent;
	};
}