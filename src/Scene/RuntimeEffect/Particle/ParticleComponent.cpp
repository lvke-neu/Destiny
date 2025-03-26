#include "ParticleComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "ParticleEmitter.h"

namespace Destiny
{
	ParticleComponent::ParticleComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/particle_rain.hlsl");
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

	void ParticleComponent::onUpdate(float deltaTime)
	{
		updateParticles(deltaTime);
		updateMesh();
	}

	void ParticleComponent::initParticles()
	{
		m_particles.resize(100);
		for (int i = 0; i < m_particles.size(); i++)
		{
			m_particles[i] = ParticleEmitter::GetInstance()->emit();
		}
	}

	void ParticleComponent::updateParticles(float deltaTime)
	{
		for (int i = 0; i < m_particles.size(); i++)
		{
			if (m_particles[i].age > 2.0f)
			{
				m_particles[i] = ParticleEmitter::GetInstance()->emit();
				continue;
			}

			m_particles[i].position.x += -1.0f * deltaTime;
			m_particles[i].position.y += m_particles[i].velocity.y * deltaTime + 0.5f * -9.8f * deltaTime * deltaTime;
			m_particles[i].velocity.y += -9.8f * deltaTime;
			m_particles[i].age += deltaTime;
		}
	}

	void ParticleComponent::updateMesh()
	{
		std::shared_ptr<Blob> data = nullptr;
		std::vector<DirectX::XMFLOAT3> vertices;

		for (const auto& particle : m_particles)
		{
			vertices.push_back({ particle.position.x + 0.05f, particle.position.y + 0.5f, particle.position.z });
			vertices.push_back({ particle.position.x, particle.position.y - 0.5f, particle.position.z });
		}

		data.reset(new Blob(vertices.size() * sizeof(DirectX::XMFLOAT3)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(DirectX::XMFLOAT3));
		auto vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_Position3(), (unsigned int)sizeof(DirectX::XMFLOAT3), 0, data);

		auto mesh = getMesh();
		if (!mesh)
		{
			DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };
			Mesh::DrawCall drawCall;
			drawCall.drawMethod = Mesh::DrawMethod::Draw;
			drawCall.primitiveTopology = Mesh::PrimitiveTopology::LineList;
			drawCall.indexCount = 0;
			drawCall.vertexCount = (unsigned int)vertices.size();

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, nullptr);
			mesh->load(0);

			setMesh(mesh);
		}
		else
		{
			mesh->modifyVertexBuffer(vertexBuffer);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ParticleComponent>("ParticleComponent")
			.constructor<>();
	}
}