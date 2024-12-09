#pragma once
#include "Component.h"
#include "Graphics/Visual.h"

namespace Destiny
{
	class VisualComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		VisualComponent();
		virtual ~VisualComponent() = default;
	public:
		GET_CLASS_NAME(VisualComponent);
	public:
		virtual void onAddToNode() override;
		virtual void onNodeTransformChanged() override;
		virtual void onEnterScene();

		void onCameraViewChanged(const DirectX::XMMATRIX& cameraView);
		void onCameraProjChanged(const DirectX::XMMATRIX& cameraProj, float viewportWidth, float viewportHeight);
	public:
		void setRenderPass(std::shared_ptr<RenderPass> renderPass);
		void setMesh(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Visual> getVisual();
	private:
		std::shared_ptr<Visual> m_visual;
	};

	inline void VisualComponent::setRenderPass(std::shared_ptr<RenderPass> renderPass)
	{
		if (m_visual)
		{
			m_visual->setRenderPass(renderPass);
		}	
	}

	inline void VisualComponent::setMesh(std::shared_ptr<Mesh> mesh)
	{
		if (m_visual)
		{
			m_visual->setMesh(mesh);
		}
	}

	inline std::shared_ptr<Visual> VisualComponent::getVisual()
	{
		return m_visual;
	}
}