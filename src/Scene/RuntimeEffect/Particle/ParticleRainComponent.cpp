#include "ParticleRainComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"

namespace Destiny
{
	ParticleRainComponent::ParticleRainComponent()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/particle_rain.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		setRenderPass(renderPass);

		initParticles();
		updateMesh();
	}

	void ParticleRainComponent::onUpdate(float deltaTime)
	{

	}

	void ParticleRainComponent::initParticles()
	{
		m_particles.resize(100);
		for (const auto& particle : m_particles)
		{

		}
	}

	void ParticleRainComponent::updateMesh()
	{
		std::shared_ptr<Blob> data = nullptr;
		std::vector<DirectX::XMFLOAT3> vertices =
		{
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 10.0f, 0.0f }
		};

		data.reset(new Blob(vertices.size() * sizeof(DirectX::XMFLOAT3)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(DirectX::XMFLOAT3));
		auto vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(DirectX::XMFLOAT3), 0, data);


		DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::Draw;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::LineList;
		drawCall.indexCount = 0;
		drawCall.vertexCount = (unsigned int)vertices.size();

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, nullptr);
		mesh->load();

		setMesh(mesh);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ParticleRainComponent>("ParticleRainComponent")
			.constructor<>();
	}
}