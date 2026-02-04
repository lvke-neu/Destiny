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
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Particle.h"
#include <random>

namespace Destiny
{
	ParticleFireComponent::ParticleFireComponent() :
		m_maxParticlesCount(10000), // Increased particle count for better fire
		m_totalTime(0.0f),
		m_particleUpdateCS(std::make_shared<ComputerCommand>())
	{
		// Setup Compute Shader
		m_particleUpdateCS->setComputerEffectPath("builtin://renderer/particle_fire_update.hlsl");
		m_particleUpdateCS->setDebugName(L"ParticleUpdate");
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(m_particleUpdateCS);

		initResources();
	}

	ParticleFireComponent::~ParticleFireComponent()
	{
		// Cleanup if necessary (Smart pointers usually handle this, but good to be explicit if registered somewhere)
		// std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->removeBeforePipelineCommand(m_particleUpdateCS);
	}

	void ParticleFireComponent::initResources()
	{
		// 1. Create Structured Buffer (Shared between CS and VS)
		// Use sizeof(Particle) strictly
		m_particlesBuffer = Texture::CreateStructured(sizeof(Particle), m_maxParticlesCount * sizeof(Particle));
		m_particlesBuffer->load();

		// 2. Setup Compute Shader bindings
		unsigned int numGroups = (m_maxParticlesCount + 63) / 64;
		std::vector<std::shared_ptr<Texture>> uavs = { m_particlesBuffer };
		m_particleUpdateCS->setUnorderedAccessViews(uavs, { 0 });
		m_particleUpdateCS->setThreadGroupCount(numGroups, 1, 1);

		// 3. Setup Render (Visual) Pipeline
		auto renderer = Renderer::Create("builtin://renderer/particle_fire_render.hlsl");
		renderer->load(0);
		
		// Bind the same buffer as SRV for rendering
		setShaderResource("particles", m_particlesBuffer);
		
		// Load Fire Texture (Use flare texture which is common for particles)
		auto particleTexture = Texture::Create("builtin://texture/flarealpha.dds"); 
		particleTexture->load(0);
		setShaderResource("particleTexture", particleTexture);

		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load(0);
		setSamplerSate("samplerState", samplerState);

		// 4. Create Dummy Mesh for DrawCall (VertexID based)
		// We don't need actual vertex data, just the vertex count.
		DirectX::BoundingBox aabb{ { -10.0f, 0.0f, -10.0f },{ 10.0f, 20.0f, 10.0f } }; // Approximate bounds
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::Draw;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::PointList;
		drawCall.indexCount = 0;
		drawCall.vertexCount = m_maxParticlesCount; // Draw N points, expanded by GS

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, nullptr, nullptr);
		mesh->load();
		setMesh(mesh);

		// 5. Setup Render States (Additive Blending)
		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE; // Double sided
		renderStates->getDepthStencilStateDesc()->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // Don't write depth
		
		// Additive Blend: SrcAlpha + One
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendEnable = true;
		renderStates->getBlendStateDesc()->RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderStates->getBlendStateDesc()->RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		renderStates->getBlendStateDesc()->RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		renderStates->getBlendStateDesc()->RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Transparent); // Render in transparent pass
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		setRenderPass(renderPass);
	}

	void ParticleFireComponent::onUpdate(float deltaTime)
	{
		m_totalTime += deltaTime;
		m_particleUpdateCS->setConstant("deltaTime", deltaTime);
		m_particleUpdateCS->setConstant("totalTime", m_totalTime);
		
		// Ensure emitter position follows the node
		if (auto node = m_node.lock())
		{
			m_particleUpdateCS->setConstant("emitterPosition", node->get_translation());
		}
	}

	void ParticleFireComponent::onEnterScene()
	{
		VisualComponent::onEnterScene();
		if (auto node = m_node.lock())
		{
			m_particleUpdateCS->setConstant("emitterPosition", node->get_translation());
		}
	}

	void ParticleFireComponent::onPropertyChanged(const std::string& property)
	{
		if (property == "NodeTransform")
		{
			// Position update is handled in onUpdate now for smoother movement
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ParticleFireComponent>("ParticleFireComponent")
			.constructor<>();
	}
}
