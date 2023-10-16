#pragma once

struct ID3D11CommandList;
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
		void tick();
	private:
		ID3D11CommandList* m_commandList;
	};
}