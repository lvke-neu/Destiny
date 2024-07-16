#include "Scene.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/Effect.h"
#include "Graphics/EffectPass.h"
#include "Graphics/EffectTechnique.h"
#include "Graphics/RenderParameters.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Visual.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include "VisualComponent.h"
#include "CameraComponent.h"
#include <d3d11.h>

namespace Destiny
{
	Scene::Scene() : Node("Root")
	{
		
	}

	Scene::~Scene()
	{

	}

	void Scene::initialize()
	{
		//camera
		m_cameraNode = std::make_shared<Node>("Camera");
		m_camera = std::make_shared<CameraComponent>();
		m_cameraNode->addComponent(m_camera);
		m_cameraNode->addToParent(shared_from_this());

		//Effect
		auto renderer = Renderer::Create("builtin://renderer/basic.rdr");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load(0);

		std::shared_ptr<EffectPass> effectPass = std::make_shared<EffectPass>();
		effectPass->setRenderer(renderer);
		effectPass->setRenderStates(renderStates);

		std::shared_ptr<EffectTechnique> effectTechnique = std::make_shared<EffectTechnique>();
		effectTechnique->addEffectPass(effectPass);

		std::shared_ptr<Effect> effect = std::make_shared<Effect>();
		effect->addEffectTechnique(effectTechnique);

		auto boxMesh = Mesh::Create_Box_PositionNormalTexcoord();
		boxMesh->load(0);
		std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
		visualComponent->setEffect(effect);
		visualComponent->setMesh(boxMesh);


		auto boxNode = std::make_shared<Node>();
		boxNode->addComponent(visualComponent);
		boxNode->addToParent(shared_from_this());
		
		Transform transform;
		transform.set_translation({ 0.0f, 0.0f, 2.0f });
		transform.set_rotation({ 0.0f, 0.0f, 0.0f });
		transform.set_scale({ 1.0f, 1.0f, 1.0f });
		boxNode->set_transform(transform);
	}

	void Scene::uninitialize()
	{

	}

	void Scene::update(float deltaTime)
	{

	}
}