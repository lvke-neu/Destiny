#pragma once
#include "Engine/Asset.h"
#include "Graphics/GraphicsDefine.h"
#include <unordered_set>
#include <DirectXCollision.h>
#include <d3d11.h>

namespace Destiny
{
	class Animator;
	class Node;
	class ModelLoader;
	class VisualComponent;
	class Model : public Asset
	{
		friend ModelLoader;
	public:
		Model();
		virtual ~Model();
	public:
		std::shared_ptr<Node>	getNode();
		void					onDataLoaded();
		void					setEnable(bool enable);
		void					setEnableShadow(bool enableShadow);
		void					setRenderer(std::string renderer);
		void					setShadowRenderer(std::string renderer);
		void					setRendererCategory(RendererCategory rendererCategory);
		void					setRasterizerDesc(D3D11_RASTERIZER_DESC desc);

		void					updateAnimation(float deltaTime);
	public:
		std::string get_animation();
		void set_animation(std::string animation);
		unsigned int get_animationIndex();
		void set_animationIndex(unsigned int animationIndex);
		unsigned int get_animationCount();
		void set_animationCount(unsigned int animationCount);
	public:
		static std::shared_ptr<ModelLoader> s_modelLoader;
		static std::shared_ptr<Model> Create(const char* path);
	private:
		std::shared_ptr<Node> m_node;
		std::unordered_set<std::shared_ptr<VisualComponent>> m_visualComponents;
		DirectX::BoundingBox m_mergedAABB;

		std::shared_ptr<Animator> m_animator;
	};

	inline std::shared_ptr<Node> Model::getNode()
	{
		return m_node;
	}
}