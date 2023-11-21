#pragma once
#include <memory>

namespace Destiny
{
	class Visual3D;
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
	public:
		void initialize();
		void uninitialize();
		void update();
		
		//TODO:remove
		void additionalCommands();

	private:
		std::shared_ptr<Visual3D> m_visual3D;
	};
}