#include "SceneStatisticsComponent.h"
#include "TextComponent.h"
#include "Graphics/GraphicsSystem.h"
#include "Engine/Node.h"
#include <sstream>
#include <iomanip>

namespace Destiny
{
	SceneStatisticsComponent::SceneStatisticsComponent() :
		m_frameTextComponent(std::make_shared<TextComponent>()),
		m_fpsTextComponent(std::make_shared<TextComponent>()),
		m_drawcallCountTextComponent(std::make_shared<TextComponent>()),
		m_triangleCountTextComponent(std::make_shared<TextComponent>()),
		m_visualCountTextComponent(std::make_shared<TextComponent>())
	{
		m_frameTextComponent->set_screenPosition({ -0.8f, 0.9f });
		m_frameTextComponent->set_size({ 200.0f, 50.0f });
		m_frameTextComponent->set_color({ 83.0f / 255.0f,168.0f / 255.0f,253.0f / 255.0f, 1.0f });

		m_fpsTextComponent->set_screenPosition({ -0.8f, 0.75f });
		m_fpsTextComponent->set_size({ 200.0f, 50.0f });
		m_fpsTextComponent->set_color({ 83.0f / 255.0f,168.0f / 255.0f,253.0f / 255.0f, 1.0f });

		m_drawcallCountTextComponent->set_screenPosition({ -0.8f, 0.6f });
		m_drawcallCountTextComponent->set_size({ 250.0f, 50.0f });
		m_drawcallCountTextComponent->set_color({ 83.0f / 255.0f,168.0f / 255.0f,253.0f / 255.0f, 1.0f });

		m_triangleCountTextComponent->set_screenPosition({ -0.8f, 0.45f });
		m_triangleCountTextComponent->set_size({ 250.0f, 50.0f });
		m_triangleCountTextComponent->set_color({ 83.0f / 255.0f,168.0f / 255.0f,253.0f / 255.0f, 1.0f });

		m_visualCountTextComponent->set_screenPosition({ -0.8f, 0.3f });
		m_visualCountTextComponent->set_size({ 250.0f, 50.0f });
		m_visualCountTextComponent->set_color({ 83.0f / 255.0f,168.0f / 255.0f,253.0f / 255.0f, 1.0f });

		set_updateCategory(UpdateCategory::late_update);
	}

	SceneStatisticsComponent::~SceneStatisticsComponent()
	{

	}

	void SceneStatisticsComponent::onAddToNode()
	{
		if (m_node)
		{
			m_node->addComponent(m_frameTextComponent);
			m_node->addComponent(m_fpsTextComponent);
			m_node->addComponent(m_drawcallCountTextComponent);
			m_node->addComponent(m_triangleCountTextComponent);
			m_node->addComponent(m_visualCountTextComponent);

		}
	}

	void SceneStatisticsComponent::onEnterScene()
	{
		m_frameTextComponent->set_enable(true);
		m_fpsTextComponent->set_enable(true);
		m_drawcallCountTextComponent->set_enable(true);
		m_triangleCountTextComponent->set_enable(true);
		m_visualCountTextComponent->set_enable(true);
	}

	void SceneStatisticsComponent::onLeaveScene()
	{
		m_frameTextComponent->set_enable(false);
		m_fpsTextComponent->set_enable(false);
		m_drawcallCountTextComponent->set_enable(false);
		m_triangleCountTextComponent->set_enable(false);
		m_visualCountTextComponent->set_enable(false);
	}

	void SceneStatisticsComponent::onUpdate(float deltaTime)
	{
		static bool first = true;
		static float sumTime = 0.0f;
		sumTime += deltaTime;

		std::stringstream ss;
		if (first || (!first && sumTime > 1.0f))
		{
			ss << "F r a m e : " << int(deltaTime * 1000.0f) << "ms";
			m_frameTextComponent->set_text(ss.str());

			ss.str("");
			ss << "F p s : " << (int)(1 / deltaTime);
			m_fpsTextComponent->set_text(ss.str());

			sumTime = 0.0f;
			first = false;
		}

		ss.str("");
		ss << "D r a w C a l l C o u n t : " << Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().DrawCallCount;
		m_drawcallCountTextComponent->set_text(ss.str());

		ss.str("");
		ss << "T r i a n g l e C o u n t : " << Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().TriangleCount;
		m_triangleCountTextComponent->set_text(ss.str());

		ss.str("");
		ss << "V i s u a l C o u n t : " << Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().VisualCount;
		m_visualCountTextComponent->set_text(ss.str());
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TextComponent>("SceneStatisticsComponent")
			.constructor<>();
	}
}