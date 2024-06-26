#include "SceneManager.h"
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
#include "Graphics/Visual3D.h"
#include "Graphics/Texture.h"
#include <d3d11.h>

namespace Destiny
{
	SceneManager::SceneManager()
	{
		
	}

	SceneManager::~SceneManager()
	{

	}	

	std::shared_ptr<Visual3D> visual3d = nullptr;
	std::vector< std::shared_ptr<Texture>> vec;
	void SceneManager::initialize()
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
		std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>("builtin://renderer/basic.rdr");
		renderer->load(0);
		renderer->setConstant("a", 1.0f);
		renderer->setConstant("b", 0.9f);
		
		for (int i = 0; i < 1000; i++)
		{
			auto texture = Texture::Create("builtin://texture/box_diffuse.png");
			texture->load();
			vec.push_back(texture);
		}


		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load(0);

		std::shared_ptr<EffectPass> effectPass = std::make_shared<EffectPass>();
		effectPass->setRenderer(renderer);
		effectPass->setRenderStates(renderStates);

		std::shared_ptr<Renderer> renderer2 = std::make_shared<Renderer>("builtin://renderer/basic2.rdr");
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

		//Visual3D
		visual3d = std::make_shared<Visual3D>();
		visual3d->setEffect(effect);
		visual3d->setMesh(mesh);
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::update()
	{
		Engine::GetInstance()->getGraphicsSystem()->commitRenderParameters(visual3d->getRenderParameters());
	}
}