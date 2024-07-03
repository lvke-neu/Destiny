#pragma once

namespace Destiny
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	public:
		void initialize();
		void uninitialize();
		void update(float deltaTime);
	};
}