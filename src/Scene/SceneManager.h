#pragma once
#include <memory>
#include <vector>

namespace Destiny
{
	class VisualScene;
	class TextComponent;
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
	public:
		void							initialize();
		void							uninitialize();
		void							update(float deltaTime);
	public:
		std::shared_ptr<VisualScene>	getScene();
	private:
		std::shared_ptr<VisualScene>	m_scene;
		std::vector<std::shared_ptr<TextComponent>> m_texts;
	};

	inline std::shared_ptr<VisualScene> SceneManager::getScene()
	{
		return m_scene;
	}
}