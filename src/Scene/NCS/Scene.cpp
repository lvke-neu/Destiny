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
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Visual.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include "VisualComponent.h"
#include "CameraComponent.h"
#include "CameraController.h"
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
		m_cameraNode->addComponent(std::make_shared<CameraController>());
		m_cameraNode->addToParent(shared_from_this());



		//box
		{
			auto renderer = std::make_shared<Renderer>("builtin://renderer/basic.rdr");
			renderer->load(0);
			renderer->setConstant("u_color", DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f });

			std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
			renderStates->load();

			std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
			renderPass->setPipeline(RenderPass::ForwardOpaque);
			renderPass->setRenderer(renderer);
			renderPass->setRenderStates(renderStates);

			auto mesh = Mesh::Create_Box_PositionNormalTexcoord();
			mesh->load();

			std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
			visualComponent->setRenderPass(renderPass);
			visualComponent->setMesh(mesh);

			auto node = std::make_shared<Node>();
			node->addComponent(visualComponent);
			node->addToParent(shared_from_this());

			Transform transform;
			transform.set_translation({ 0.0f, 0.0f, 5.0f });
			//transform.set_rotation({ 45.0f, 0.0f, 0.0f });
			transform.set_scale({ 1.0f, 1.0f, 1.0f });
			node->set_transform(transform);
		}


		////plane
		//{
		//	auto texture = Texture::Create("builtin://texture/box_diffuse.png");
		//	texture->load();
		//	auto sampler = std::make_shared<SamplerState>();
		//	sampler->load(0);

		//	auto renderer = std::make_shared<Renderer>("builtin://renderer/basic2.rdr");
		//	renderer->load(0);
		//	renderer->setShaderResource("t_baseColor", texture);
		//	renderer->setSamplerSate("s_sampler", sampler);

		//	std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		//	renderStates->load(0);

		//	std::shared_ptr<EffectPass> effectPass = std::make_shared<EffectPass>();
		//	effectPass->setRenderer(renderer);
		//	effectPass->setRenderStates(renderStates);


		//	auto renderer2 = std::make_shared<Renderer>("builtin://renderer/basic3.rdr");
		//	renderer2->load(0);
		//	renderer2->setConstant("u_color", DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 0.5f });
		//	std::shared_ptr<RenderStates> renderStates2 = std::make_shared<RenderStates>();
		//	renderStates2->getBlendStateDesc()->RenderTarget[0].BlendEnable = true;
		//	renderStates2->getBlendStateDesc()->RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		//	renderStates2->getBlendStateDesc()->RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		//	renderStates2->getBlendStateDesc()->RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//	renderStates2->getRasterizerStateDesc()->DepthBias = -5;
		//	renderStates2->load(0);
		//	std::shared_ptr<EffectPass> effectPass2 = std::make_shared<EffectPass>();
		//	effectPass2->setRenderer(renderer2);
		//	effectPass2->setRenderStates(renderStates2);



		//	std::shared_ptr<EffectTechnique> effectTechnique = std::make_shared<EffectTechnique>();
		//	effectTechnique->addEffectPass(effectPass);
		//	effectTechnique->addEffectPass(effectPass2);

		//	std::shared_ptr<Effect> effect = std::make_shared<Effect>();
		//	effect->addEffectTechnique(effectTechnique);

		//	auto mesh = Mesh::Create_Plane_PositionNormalTexcoord();
		//	mesh->load(0);

		//	std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
		//	visualComponent->setEffect(effect);
		//	visualComponent->setMesh(mesh);

		//	auto node = std::make_shared<Node>();
		//	node->addComponent(visualComponent);
		//	node->addToParent(shared_from_this());

		//	Transform transform;
		//	transform.set_translation({ 0.0f, -2.0f, 0.0f });
		//	transform.set_rotation({ 90.0f, 0.0f, 0.0f });
		//	transform.set_scale({ 5.0f, 5.0f, 1.0f });
		//	node->set_transform(transform);
		//}
	}

	void Scene::uninitialize()
	{

	}

	void Scene::update(float deltaTime)
	{

	}
}