#include "ParticleFireComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/SamplerState.h"
#include "Engine/Node.h"
#include "Particle.h"
#include <random>

namespace Destiny
{
	ParticleFireComponent::ParticleFireComponent() :
		m_maxParticlesCount(1000),
		m_particleUpdateCS(std::make_shared<ComputerCommand>())
	{
		m_particleUpdateCS->setComputerEffectPath("builtin://renderer/particle_fire_update.hlsl");
		m_particleUpdateCS->setDebugName(L"ParticleUpdate");
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(m_particleUpdateCS);
	}

	void ParticleFireComponent::onUpdate(float deltaTime)
	{
		m_particleUpdateCS->setConstant("deltaTime", deltaTime);
	}

	void ParticleFireComponent::onPropertyChanged(const std::string& property)
	{
		if (property == "NodeTransform")
		{
			m_particleUpdateCS->setConstant("emitterPosition", m_node->get_translation());
			update();
		}
	}

	void ParticleFireComponent::update()
	{
		//cs
		{
			m_particlesBuffer = Texture::CreateStructured(sizeof(Particle), m_maxParticlesCount * sizeof(Particle));
			m_particlesBuffer->load();

			unsigned int numGroups = (m_maxParticlesCount + 63) / 64;
			std::vector<std::shared_ptr<Texture>> uavs = { m_particlesBuffer };
			m_particleUpdateCS->setUnorderedAccessViews(uavs);
			m_particleUpdateCS->setThreadGroupCount(numGroups, 1, 1);
		}

		//visual
		{
			auto renderer = Renderer::Create("builtin://renderer/particle_fire_render.hlsl");
			renderer->load(0);
			setShaderResource("particles", m_particlesBuffer);
			//auto particleTexture = Texture::Create("builtin://texture/particle/whiteDot.png");
			//particleTexture->load();
			//setShaderResource("particleTexture", particleTexture);
			auto samplerState = std::make_shared<SamplerState>();
			samplerState->load();
			setSamplerSate("samplerState", samplerState);

			DirectX::BoundingBox aabb{ { 0.0f, 0.0f, 0.0f },{ FLT_MAX, FLT_MAX, FLT_MAX } };
			Mesh::DrawCall drawCall;
			drawCall.drawMethod = Mesh::DrawMethod::Draw;
			drawCall.primitiveTopology = Mesh::PrimitiveTopology::PointList;
			drawCall.indexCount = 0;
			drawCall.vertexCount = m_maxParticlesCount;

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, nullptr, nullptr);
			mesh->load();
			setMesh(mesh);
		

			std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
			renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE;

			renderStates->load();

			std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
			renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
			renderPass->setRenderer(renderer);
			renderPass->setRenderStates(renderStates);

			setRenderPass(renderPass);
		}
	}

	
	RTTR_REGISTRATION
	{
		rttr::registration::class_<ParticleFireComponent>("ParticleFireComponent")
			.constructor<>();
	}
}