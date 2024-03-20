#pragma once

namespace Destiny
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
	public:
		void initialize();
		void uninitialize();
		void update();
	};

}