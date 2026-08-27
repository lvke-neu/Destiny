#include "RendererManager.h"

namespace Destiny
{
	RendererManager::RendererManager()
	{

	}

	RendererManager::~RendererManager()
	{

	}

	std::shared_ptr<Renderer> RendererManager::create(const char* path)
	{
		auto iter = m_cache.find(path);
		if (iter != m_cache.end())
		{
			return iter->second;
		}

		std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(path);

		m_cache.insert({ path, renderer });
		return renderer;
	}
}