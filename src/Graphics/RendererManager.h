#pragma once
#include "Renderer.h"
#include "Engine/Blob.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace Destiny
{
	class RendererManager
	{
	public:
		RendererManager();
		~RendererManager();
	public:
		std::shared_ptr<Renderer> create(const char* path);
		template<typename T>
		void							setConstant(const char* name, T value);
		void							setConstant(const char* name, std::shared_ptr<Blob> blob);
		void							setShaderResource(const char* name, std::shared_ptr<Texture> texture);
		void							setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState);
	private:
		std::unordered_map<std::string, std::shared_ptr<Renderer>> m_cache;
	};

	template<typename T>
	inline void RendererManager::setConstant(const char* name, T value)
	{
		for (const auto& renderer : m_cache)
		{
			renderer.second->setConstant(name, value);
		}
	}

	inline void RendererManager::setConstant(const char* name, std::shared_ptr<Blob> blob)
	{
		for (const auto& renderer : m_cache)
		{
			renderer.second->setConstant(name, blob);
		}
	}

	inline void RendererManager::setShaderResource(const char* name, std::shared_ptr<Texture> texture)
	{
		for (const auto& renderer : m_cache)
		{
			renderer.second->setShaderResource(name, texture);
		}
	}

	inline void RendererManager::setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState)
	{
		for (const auto& renderer : m_cache)
		{
			renderer.second->setSamplerSate(name, samplerState);
		}
	}
}