#pragma once
#include "Engine/Component.h"
#include "Graphics/GraphicsDefine.h"
#include <d3d11.h>

namespace Destiny
{
	class Model;
	class ModelComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		ModelComponent();
		virtual ~ModelComponent();
	public:
		GET(std::string, path);
		void set_path(std::string path);
		GET(std::string, renderer);
		void set_renderer(std::string renderer);
		GET(std::string, shadowRenderer);
		void set_shadowRenderer(std::string shadowRenderer);
		GET(RendererCategory, rendererCategory);
		void set_rendererCategory(RendererCategory rendererCategory);
		GET(D3D11_RASTERIZER_DESC, rasterizerDesc);
		void set_rasterizerDesc(D3D11_RASTERIZER_DESC rasterizerDesc);
		GET(bool, enableShadow);
		void set_enableShadow(bool enableShadow);

	public:
		std::string get_animation();
		void set_animation(std::string animation);
		unsigned int get_animationIndex();
		void set_animationIndex(unsigned int animationIndex);
		unsigned int get_animationCount();
		void set_animationCount(unsigned int animationCount);
	private:
		virtual void onUpdate(float deltaTime) override;
		virtual void onPropertyChanged(const std::string& property) override;
	private:
		std::string				m_path;
		std::shared_ptr<Model>	m_model;
		bool					m_modelChanged;
		std::string				m_renderer;
		std::string				m_shadowRenderer;
		RendererCategory		m_rendererCategory;
		D3D11_RASTERIZER_DESC   m_rasterizerDesc;
		bool m_enableShadow;

		std::string m_animation;
		unsigned int m_animationIndex;
	};
}