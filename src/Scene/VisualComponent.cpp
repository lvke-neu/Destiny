#include "VisualComponent.h"
#include "NCS/Node.h"
#include "Graphics/Effect.h"
#include "Graphics/EffectTechnique.h"
#include "Graphics/EffectPass.h"
#include "Graphics/Renderer.h"

namespace Destiny
{
	VisualComponent::VisualComponent() : 
		m_visual(std::make_shared<Visual>())
	{

	}

	void VisualComponent::onAddToNode(std::shared_ptr<Node> node)
	{
		if (!node || !m_visual->getEffect())
		{
			return;
		}

		onNodeTransformChanged(node->get_transform());
	}

	void VisualComponent::onNodeTransformChanged(const Transform& transform)
	{
		for (const auto& effectTechnique : m_visual->getEffect()->getEffectTechniques())
		{
			for (const auto& effectPass : effectTechnique->getEffectPasses())
			{
				auto renderer = effectPass->getRenderer();
				if (renderer)
				{
					renderer->setConstant("u_worldMatrix", transform.getTransposeWorldMatrix());
				}
			}
		}
	}

	void VisualComponent::onCameraViewChanged(const DirectX::XMMATRIX& cameraView)
	{
		if (!m_visual->getEffect())
		{
			return;
		}

		for (const auto& effectTechnique : m_visual->getEffect()->getEffectTechniques())
		{
			for (const auto& effectPass : effectTechnique->getEffectPasses())
			{
				auto renderer = effectPass->getRenderer();
				if (renderer)
				{
					renderer->setConstant("g_view", cameraView);
				}
			}
		}
	}

	void VisualComponent::onCameraProjChanged(const DirectX::XMMATRIX& cameraProj)
	{
		if (!m_visual->getEffect())
		{
			return;
		}

		for (const auto& effectTechnique : m_visual->getEffect()->getEffectTechniques())
		{
			for (const auto& effectPass : effectTechnique->getEffectPasses())
			{
				auto renderer = effectPass->getRenderer();
				if (renderer)
				{
					renderer->setConstant("g_proj", cameraProj);
				}
			}
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualComponent>("VisualComponent")
			.constructor<>();
	}
}