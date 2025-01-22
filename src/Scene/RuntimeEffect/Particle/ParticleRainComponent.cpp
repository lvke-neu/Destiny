#include "ParticleRainComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include <random>

namespace Destiny
{
	ParticleRainComponent::ParticleRainComponent() :
		m_particleMaxAge(2.0f),
		m_gravity(-9.8f)
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
		updateParticles(deltaTime);
		updateMesh();
	}

	void ParticleRainComponent::initParticles()
	{
		m_particles.resize(100);
		for (int i = 0; i < m_particles.size(); i++)
		{
			m_particles[i].position = { -10.0f + i / 4.0f, randomFloat(20.0f, 30.0f), 0.0f };
			m_particles[i].velocity = { 0.0f, randomFloat(-20.0f, -10.0f), 0.0f };
			m_particles[i].age = randomFloat(0.0f, m_particleMaxAge);
		}
	}

	void ParticleRainComponent::updateParticles(float deltaTime)
	{
		for (int i = 0; i < m_particles.size(); i++)
		{
			if (m_particles[i].age > m_particleMaxAge)
			{
				m_particles[i].position = { -10.0f + i / 4.0f, randomFloat(20.0f, 30.0f), 0.0f };
				m_particles[i].velocity = { 0.0f, randomFloat(-20.0f, -10.0f), 0.0f };
				m_particles[i].age = randomFloat(0.0f, m_particleMaxAge);
				continue;
			}
			
			m_particles[i].position.y += m_particles[i].velocity.y * deltaTime + 0.5f * m_gravity * deltaTime * deltaTime;
			m_particles[i].velocity.y += m_gravity * deltaTime;
			m_particles[i].age += deltaTime;
		}
	}

	void ParticleRainComponent::updateMesh()
	{
		std::shared_ptr<Blob> data = nullptr;
		std::vector<DirectX::XMFLOAT3> vertices;
		
		for (const auto& particle : m_particles)
		{
			vertices.push_back({ particle.position.x, particle.position.y + 0.5f, particle.position.z });
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

	float ParticleRainComponent::randomFloat(float min, float max)
	{
		std::random_device rd;
		std::default_random_engine engine(rd());

		std::uniform_real_distribution<float> distribution(min, max);

		return distribution(engine);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ParticleRainComponent>("ParticleRainComponent")
			.constructor<>()
			.property("particleMaxAge", &ParticleRainComponent::get_particleMaxAge, &ParticleRainComponent::set_particleMaxAge)
			.property("gravity", &ParticleRainComponent::get_gravity, &ParticleRainComponent::set_gravity);
	}
}