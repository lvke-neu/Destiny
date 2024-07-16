#pragma once
#include "Component.h"
#include "Graphics/Visual.h"

namespace Destiny
{
	class Effect;
	class Mesh;
	class RenderParameters;
	class VisualComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		VisualComponent();
		virtual ~VisualComponent() = default;
	public:
		GET_CLASS_NAME(VisualComponent);
	public:
		virtual void onAddToNode(std::shared_ptr<Node> node) override;
		virtual void onNodeTransformChanged(const Transform& transform) override;
		virtual void onEnterScene(std::shared_ptr<Scene> scene);

		void onCameraViewChanged(const DirectX::XMMATRIX& cameraView);
		void onCameraProjChanged(const DirectX::XMMATRIX& cameraProj);
	public:
		void setEffect(std::shared_ptr<Effect> effect);
		void setMesh(std::shared_ptr<Mesh> mesh);
		const std::unordered_set<std::shared_ptr<RenderParameters>>& getRenderParameters();
	private:
		std::shared_ptr<Visual> m_visual;
	};

	inline void VisualComponent::setEffect(std::shared_ptr<Effect> effect)
	{
		if (m_visual)
		{
			m_visual->setEffect(effect);
		}	
	}

	inline void VisualComponent::setMesh(std::shared_ptr<Mesh> mesh)
	{
		if (m_visual)
		{
			m_visual->setMesh(mesh);
		}
	}

	static std::unordered_set<std::shared_ptr<RenderParameters>> nullRenderParameters;
	inline const std::unordered_set<std::shared_ptr<RenderParameters>>& VisualComponent::getRenderParameters()
	{
		if (m_visual)
		{
			return m_visual->getRenderParameters();
		}

		return nullRenderParameters;
	}
}