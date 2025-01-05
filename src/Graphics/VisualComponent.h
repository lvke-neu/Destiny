#pragma once
#include "Visual.h"
#include "Engine/Component.h"
#include <DirectXMath.h>

namespace Destiny
{
	class VisualComponent : public Component, public std::enable_shared_from_this<VisualComponent>
	{
		RTTR_ENABLE(Component);
	public:
		VisualComponent();
		virtual ~VisualComponent() = default;
	public:
		virtual void				onAddToNode() override;
		virtual void				onNodeTransformChanged() override;
		virtual void				onEnterScene();

		void						onCameraViewChanged(const DirectX::XMMATRIX& cameraView, const DirectX::XMFLOAT3& eyePosition);
		void						onCameraProjChanged(const DirectX::XMMATRIX& cameraProj, float viewportWidth, float viewportHeight);
		void						onLightDirectionChanged(const DirectX::XMFLOAT3& direction);
	public:
		void						setRenderPass(std::shared_ptr<RenderPass> renderPass);
		void						setMesh(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Visual>		getVisual();
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