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

	void Scene::initialize()
	{
		std::shared_ptr<Blob> data = nullptr;


		//VertexBuffer
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texcoord;
		};
		std::vector<Vertex> vertices;
		vertices.resize(3);
		vertices[0] = { { -0.5f, 0.0f, 0.0f }, {0.0f, 1.0f} };
		vertices[1] = { {  0.0f, 0.5f, 0.0f }, {0.5f, 0.0f} };
		vertices[2] = { {  0.5f, 0.0f, 0.0f }, {1.0f, 1.0f} };
		data.reset(new Blob(vertices.size() * sizeof(Vertex)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(Vertex));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionTexcoord(), (unsigned int)sizeof(Vertex), 0, data);

		//IndexBuffer
		std::vector<unsigned short> indices{ 0 ,1, 2 };
		data.reset(new Blob(indices.size() * sizeof(unsigned short)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned short));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);

		//Mesh
		DirectX::BoundingBox aabb({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);
		mesh->load(0);

		//Effect
		std::shared_ptr<Renderer> renderer = Renderer::Create("builtin://renderer/basic.rdr");
		renderer->load(0);
		renderer->setConstant("a", 1.0f);
		renderer->setConstant("b", 0.1f);

		std::shared_ptr<SamplerState> samplerState = std::make_shared<SamplerState>();
		samplerState->getSamplerDesc()->Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		samplerState->load(0);
		renderer->setSamplerSate("g_ambientSampler", samplerState);

		std::shared_ptr<Texture> texture = Texture::Create("builtin://texture/box_diffuse.png");
		texture->load();
		renderer->setShaderResource("g_ambientTexture", texture);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load(0);

		std::shared_ptr<EffectPass> effectPass = std::make_shared<EffectPass>();
		effectPass->setRenderer(renderer);
		effectPass->setRenderStates(renderStates);

		std::shared_ptr<Renderer> renderer2 = Renderer::Create("builtin://renderer/basic2.rdr");
		renderer2->load(0);
		std::shared_ptr<RenderStates> renderStates2 = std::make_shared<RenderStates>();
		renderStates2->getRasterizerStateDesc()->FillMode = D3D11_FILL_WIREFRAME;
		renderStates2->load(0);
		std::shared_ptr<EffectPass> effectPass2 = std::make_shared<EffectPass>();
		effectPass2->setRenderer(renderer2);
		effectPass2->setRenderStates(renderStates2);

		std::shared_ptr<EffectTechnique> effectTechnique = std::make_shared<EffectTechnique>();
		effectTechnique->addEffectPass(effectPass);
		effectTechnique->addEffectPass(effectPass2);

		std::shared_ptr<Effect> effect = std::make_shared<Effect>();
		effect->addEffectTechnique(effectTechnique);

		std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
		visualComponent->setEffect(effect);
		visualComponent->setMesh(mesh);

		m_rootNode->addComponent(visualComponent);
	}

	void Scene::uninitialize()
	{

	}

	void Scene::update(float deltaTime)
	{
		
	}
}