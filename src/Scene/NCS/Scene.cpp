#include "Scene.h"
#include "Node.h"
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
#include "../VisualComponent.h"
#include <d3d11.h>

namespace Destiny
{
	Scene::Scene() : 
		m_rootNode(std::make_shared<Node>("RootNode"))
	{
		
	}

	Scene::~Scene()
	{

	}

	std::shared_ptr<Node> boxNode;
	void Scene::initialize()
	{
		//Effect
		std::shared_ptr<Renderer> renderer = Renderer::Create("builtin://renderer/basic.rdr");
		renderer->load(0);
		DirectX::XMFLOAT3 trans = { 0.0f, 0.0f, 13.0f };
		renderer->setConstant("u_worldMatrix", DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&trans))));
		renderer->setConstant("g_proj", DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), 284.0f / 55, 0.1f, 1000.f)));

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


		boxNode = std::make_shared<Node>();
		boxNode->addComponent(visualComponent);
		Transform transform;
		transform.set_translation({ 0.0f, 0.0f, 5.0f });
		transform.set_rotation({ 90.0f, 0.0f, 0.0f });
		transform.set_scale({ 2.0f, 1.0f, 1.0f });
		boxNode->set_transform(transform);
		boxNode->addToParent(m_rootNode);
	}

	void Scene::uninitialize()
	{

	}

	void Scene::update(float deltaTime)
	{

	}
}