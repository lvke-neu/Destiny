#include "ViewPortPanel.h"
#include "Imgui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Engine/FileSystem.h"
#include "Engine/UnSerializer.h"
#include "Engine/Blob.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderTargetView.h"
#include "Graphics/BindRenderTargets.h"
#include "Graphics/VisualScene.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Model/ModelComponent.h"
#include "Scene/SceneManager.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

ViewPortPanel::ViewPortPanel() :
	m_choosedNode(nullptr),
	m_gzimoType(0),
	m_gzimoMode(0)
{

}

void ViewPortPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport"); 

	//if (ImGui::Button("Translate"))
	//{
	//	m_gzimoType = 0;
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Rotate"))
	//{
	//	m_gzimoType = 1;
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Scale"))
	//{
	//	m_gzimoType = 2;
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Local"))
	//{
	//	m_gzimoMode = 0;
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("World"))
	//{
	//	m_gzimoMode = 1;
	//}

	//ImGui::Text("%.3f ms/frame (%.1f FPS),", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::SameLine();
	//ImGui::Text("DrawCallCount:%ld,", Destiny::Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().DrawCallCount);
	//ImGui::SameLine();
	//ImGui::Text("TriangleCount:%ld,", Destiny::Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().TriangleCount);
	//ImGui::SameLine();
	//ImGui::Text("VisualCount:%ld", Destiny::Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().VisualCount);

	processEvent();
	
	auto rtv = std::static_pointer_cast<Destiny::RenderSystem>(Destiny::Engine::GetInstance()->getGraphicsSystem())->getRenderTargetView();
	if (rtv && rtv->isLoadingSucceed())
	{
		ImGui::Image(*(rtv->getShaderResourceView()), { (float)rtv->getWidth(), (float)rtv->getHeight() });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
			{
				std::string dropPath = (char*)payload->Data;
				std::shared_ptr<Destiny::Blob> blob = nullptr;

				if (dropPath.find(".scene") != std::string::npos)
				{
					auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(dropPath.c_str());
					if (blobLoader)
					{
						auto blobHolder = blobLoader->createBlobHolder(dropPath);
						if (blobHolder)
						{
							blobHolder->load(0);
							blob = blobHolder->getBlob();
						}
					}

					if (blob)
					{
						std::string sceneContent((char*)blob->getData(), blob->getLength());
						std::shared_ptr<Destiny::Object> object = nullptr;
						Destiny::UnSerializer::UnSerialize(object, sceneContent);
						auto scene = std::dynamic_pointer_cast<Destiny::Scene>(object);
						if (scene)
						{
							scene->initialize();
							Destiny::Engine::GetInstance()->getSceneManager()->setCurrentScene(scene);
							send(ChoosedNode, nullptr);
							LOG_INFO("Open Scene:{0} Successfully", dropPath);
						}
						else
						{
							LOG_ERROR("Open Scene:{0} Failure", dropPath);
						}
					}
					else
					{
						LOG_ERROR("Open Scene:{0} Failure", dropPath);
					}
				}
				else if (dropPath.find(".obj") != std::string::npos || dropPath.find(".dae") != std::string::npos || dropPath.find(".gltf") != std::string::npos)
				{
					if (Destiny::Engine::GetInstance()->getSceneManager()->getCurrentScene())
					{
						LOG_INFO("Open Model:{0}", dropPath);

						auto node = std::make_shared<Destiny::Node>();
						auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(dropPath.c_str());
						if (blobLoader)
						{
							auto blobHolder = blobLoader->createBlobHolder(dropPath);
							node->set_name(std::experimental::filesystem::path(blobLoader->normalizedPath(blobHolder)).filename().u8string());
						}

						auto modelComponent = std::make_shared<Destiny::ModelComponent>();
						modelComponent->set_path(dropPath);
						node->addComponent(modelComponent);

						node->addToParent(Destiny::Engine::GetInstance()->getSceneManager()->getCurrentScene());
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	
	processGzimo();

	ImGui::End();
	ImGui::PopStyleVar();
}

void ViewPortPanel::processEvent()
{
	auto viewport = ImGui::GetContentRegionAvail();
	if (m_width != viewport.x || m_height != viewport.y)
	{
		m_width = (unsigned int)viewport.x;
		m_height = (unsigned int)viewport.y;
		Destiny::WindowResizeData windowResizeData;
		windowResizeData.width = m_width;
		windowResizeData.height = m_height;
		Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::WindowResize, &windowResizeData);
	}

	Destiny::Engine::GetInstance()->getEventSystem()->setViewportHovered(ImGui::IsWindowHovered());
}

void ViewPortPanel::processGzimo()
{
	if (!m_choosedNode)
	{
		return;
	}

	auto scene =std::dynamic_pointer_cast<Destiny::VisualScene>(Destiny::Engine::GetInstance()->getSceneManager()->getCurrentScene());
	if (!scene)
	{
		return;
	}

	auto camera = scene->getCamera();
	auto cameraNode = scene->getCameraNode();
	if (!cameraNode || !camera)
	{
		return;
	}

	auto viewMatrix = cameraNode->get_transform().getInverseWorldMatrix();
	auto projMatrix = camera->getProjectionMatrix();


	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();

	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

	auto fullWorldMatrix = m_choosedNode->getRootToThisWorldMatrix();
	auto ancestorsWorldMatrix = fullWorldMatrix * m_choosedNode->get_transform().getInverseWorldMatrix();
	ImGuizmo::Manipulate((float*)&viewMatrix, (float*)&projMatrix, (ImGuizmo::OPERATION)m_gzimoType, (ImGuizmo::MODE)m_gzimoMode, (float*)&fullWorldMatrix);

	if (ImGuizmo::IsUsing())
	{
		ancestorsWorldMatrix = DirectX::XMMatrixInverse(nullptr, ancestorsWorldMatrix);
		Destiny::Transform transform;
		transform.setWorldMatrix(ancestorsWorldMatrix * fullWorldMatrix);
		m_choosedNode->set_transform(transform);
	}
}

void ViewPortPanel::onChoosedNode(void* parameter)
{
	if (!parameter)
	{
		m_choosedNode = nullptr;
		return;
	}
	m_choosedNode = *(std::shared_ptr<Destiny::Node>*) parameter;
}

	