#include "VisualComponent.h"
#include "CameraComponent.h"
#include "DirectionLightComponent.h"
#include "PointLightComponent.h"
#include "VisualScene.h"
#include "RenderPass.h"
#include "Renderer.h"
#include "Engine/Scene.h"
#include "Engine/Node.h"
#include "Math/Math.h"

namespace Destiny
{
	VisualComponent::VisualComponent() : 
		m_visual(std::make_shared<Visual>())
	{

	}

	std::string VisualComponent::get_renderer()
	{
		return m_visual ? m_visual->get_renderer() : "";
	}

	void VisualComponent::set_renderer(std::string renderer)
	{
		if (m_visual)
		{
			m_visual->set_renderer(renderer);
			onEnterScene();
		}
	}

	RendererCategory VisualComponent::get_rendererCategory()
	{
		return m_visual ? m_visual->get_rendererCategory() : RendererCategory::None;
	}

	void VisualComponent::set_rendererCategory(RendererCategory rendererCategory)
	{
		if (m_visual)
		{
			m_visual->set_rendererCategory(rendererCategory);
		}
	}

	D3D11_RASTERIZER_DESC	VisualComponent::get_rasterizerDesc()
	{
		if (m_visual)
		{
			return m_visual->get_rasterizerDesc();
		}
		return D3D11_RASTERIZER_DESC();
	}

	void VisualComponent::set_rasterizerDesc(D3D11_RASTERIZER_DESC desc)
	{
		if (m_visual)
		{
			m_visual->set_rasterizerDesc(desc);
		}
	}

	D3D11_DEPTH_STENCIL_DESC VisualComponent::get_depthStencilDesc()
	{
		if (m_visual)
		{
			return m_visual->get_depthStencilDesc();
		}
		return D3D11_DEPTH_STENCIL_DESC();
	}

	void VisualComponent::set_depthStencilDesc(D3D11_DEPTH_STENCIL_DESC desc)
	{
		if (m_visual)
		{
			m_visual->set_depthStencilDesc(desc);
		}
	}

	D3D11_BLEND_DESC VisualComponent::get_blendDesc()
	{
		if (m_visual)
		{
			return m_visual->get_blendDesc();
		}
		return D3D11_BLEND_DESC();
	}

	void VisualComponent::set_blendDesc(D3D11_BLEND_DESC desc)
	{
		if (m_visual)
		{
			return m_visual->set_blendDesc(desc);
		}
	}

	void VisualComponent::reCompileShader()
	{
		if (m_visual)
		{
			m_visual->reCompileShader();
		}
	}

	std::shared_ptr<Material> VisualComponent::get_material()
	{
		if (m_visual)
		{
			return m_visual->get_material();
		}
		return nullptr;
	}

	void VisualComponent::set_material(std::shared_ptr<Material> material)
	{
		if (m_visual)
		{
			m_visual->set_material(material);
		}
	}

	void VisualComponent::onAddToNode()
	{
		if (m_visual)
		{
			m_visual->setComponent(shared_from_this());
		}
		onNodeTransformChanged();
	}

	void VisualComponent::onNodeTransformChanged()
	{
		if (!m_node || !m_visual)
		{
			return;
		}

		auto worldMatrix = m_node->getRootToThisWorldMatrix();
		m_visual->setConstant("u_worldMatrix", XMMatrixTranspose(worldMatrix));
		worldMatrix.r[3] = DirectX::g_XMIdentityR3;
		worldMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr, worldMatrix));
		m_visual->setConstant("u_worldInvTransposeMatrix", XMMatrixTranspose(worldMatrix));
	}

	void VisualComponent::onEnterScene()
	{
		auto visualScene = std::dynamic_pointer_cast<VisualScene>(m_scene);
		if (!visualScene || !visualScene->getCamera() || !visualScene->getCameraNode())
		{
			return;
		}
		onNodeTransformChanged();
		onCameraViewChanged(visualScene->getCameraNode()->get_transform().getInvTransposeWorldMatrix(), visualScene->getCameraNode()->get_transform().get_translation());
		onCameraProjChanged(
			DirectX::XMMatrixTranspose
			(
				DirectX::XMMatrixPerspectiveFovLH(visualScene->getCamera()->get_fovy() * Math::DEG2RAD, visualScene->getCamera()->get_aspect(), visualScene->getCamera()->get_nearz(), visualScene->getCamera()->get_farz())
			),
			visualScene->getCamera()->get_viewportWidth(), visualScene->getCamera()->get_viewportHeight(),
			visualScene->getCamera()->get_nearz(), visualScene->getCamera()->get_farz()
		);

		std::vector<DirectionLight> directionLights;
		traversalDirectionLight(m_scene, directionLights);
		onDirectionLightChanged(directionLights);
		std::vector<PointLight> pointLights;
		traversalPointLight(m_scene, pointLights);
		onPointLightChanged(pointLights);
	}

	void VisualComponent::onCameraViewChanged(const DirectX::XMMATRIX& cameraView, const DirectX::XMFLOAT3& eyePosition)
	{
		if (!m_visual)
		{
			return;
		}
		
		m_visual->setConstant("g_view", cameraView);
		m_visual->setConstant("g_eyePosition", eyePosition);
	}

	void VisualComponent::onCameraProjChanged(const DirectX::XMMATRIX& cameraProj, float viewportWidth, float viewportHeight, float nearPlane, float farPlane)
	{
		if (!m_visual)
		{
			return;
		}

		m_visual->setConstant("g_proj", cameraProj);
		m_visual->setConstant("g_viewportWidth", viewportWidth);
		m_visual->setConstant("g_rcpViewportWidth", 1.0f / viewportWidth);
		m_visual->setConstant("g_viewportHeight", viewportHeight);
		m_visual->setConstant("g_rcpViewportHeight", 1.0f / viewportHeight);
		m_visual->setConstant("nearPlane", nearPlane);
		m_visual->setConstant("farPlane", farPlane);
	}

	void VisualComponent::onDirectionLightChanged(const std::vector<DirectionLight>& directionLights)
	{
		if (!m_visual)
		{
			return;
		}
		std::shared_ptr<Blob> blob = std::make_shared<Blob>(directionLights.size() * sizeof(DirectionLight));
		blob->copyfrom((void*)directionLights.data(), blob->getLength());

		m_visual->setConstant("g_directionLightCount", (int)directionLights.size());
		m_visual->setConstant("g_directionLights", blob);
	}

	void VisualComponent::traversalDirectionLight(std::shared_ptr<Node> node, std::vector<DirectionLight>& directionLights)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto dlComponent = std::dynamic_pointer_cast<DirectionLightComponent>(component);
			if (dlComponent && dlComponent->get_node())
			{
				directionLights.push_back({{dlComponent->get_color()}, {dlComponent->get_node()->get_transform().get_rotation()}, dlComponent->get_intensity()});
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversalDirectionLight(childNode, directionLights);
		}
	}

	void VisualComponent::onPointLightChanged(const std::vector<PointLight>& pointLights)
	{
		if (!m_visual)
		{
			return;
		}
		std::shared_ptr<Blob> blob = std::make_shared<Blob>(pointLights.size() * sizeof(PointLight));
		blob->copyfrom((void*)pointLights.data(), blob->getLength());

		m_visual->setConstant("g_pointLightCount", (int)pointLights.size());
		m_visual->setConstant("g_pointLights", blob);
	}

	void VisualComponent::traversalPointLight(std::shared_ptr<Node> node, std::vector<PointLight>& pointLights)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto plComponent = std::dynamic_pointer_cast<PointLightComponent>(component);
			if (plComponent && plComponent->get_node())
			{
				pointLights.push_back({ {plComponent->get_color()}, {plComponent->get_node()->get_transform().get_translation()}, plComponent->get_intensity() });
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversalPointLight(childNode, pointLights);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualComponent>("VisualComponent")
			.constructor<>()
			.property("renderer", &VisualComponent::get_renderer, &VisualComponent::set_renderer)
			.property("rendererCategory", &VisualComponent::get_rendererCategory, &VisualComponent::set_rendererCategory)
			.property("rasterizerDesc", &VisualComponent::get_rasterizerDesc, &VisualComponent::set_rasterizerDesc)
			.property("depthStencilDesc", &VisualComponent::get_depthStencilDesc, &VisualComponent::set_depthStencilDesc)
			.property("blendDesc", &VisualComponent::get_blendDesc, &VisualComponent::set_blendDesc)
			.property("material", &VisualComponent::get_material, &VisualComponent::set_material);
	}
}