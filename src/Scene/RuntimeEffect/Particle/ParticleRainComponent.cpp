#include "ParticleRainComponent.h"
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
	ParticleRainComponent::ParticleRainComponent() :
		m_maxParticlesCount(10000), // Lots of rain drops
		m_totalTime(0.0f),
		m_rainAreaSize({ 20.0f, 20.0f, 20.0f }), // 20m box
		m_fallSpeed(15.0f),
		m_dropLength(0.5f),
		m_particleUpdateCS(std::make_shared<ComputerCommand>())
	{
		m_particleUpdateCS->setComputerEffectPath("builtin://renderer/particle_rain_update.hlsl");
		m_particleUpdateCS->setDebugName(L"ParticleRainUpdate");
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(m_particleUpdateCS);

		initResources();
	}

	ParticleRainComponent::~ParticleRainComponent()
	{
	}

	void ParticleRainComponent::initResources()
	{
		// 1. Buffer
		m_particlesBuffer = Texture::CreateStructured(sizeof(Particle), m_maxParticlesCount * sizeof(Particle));
		m_particlesBuffer->load();

		// 2. CS Bindings
		unsigned int numGroups = (m_maxParticlesCount + 63) / 64;
		std::vector<std::shared_ptr<Texture>> uavs = { m_particlesBuffer };
		m_particleUpdateCS->setUnorderedAccessViews(uavs, { 0 });
		m_particleUpdateCS->setThreadGroupCount(numGroups, 1, 1);

		// 3. Render Pipeline
		auto renderer = Renderer::Create("builtin://renderer/particle_rain_render.hlsl");
		renderer->load(0);
		
		setShaderResource("particles", m_particlesBuffer);
		// Rain shader doesn't strictly need a texture if we draw lines/streaks, but keeping slot 1 filled is safe
		// Or we can use a streak texture.
		auto particleTexture = Texture::Create("builtin://texture/flarealpha.dds"); 
		particleTexture->load();
		setShaderResource("particleTexture", particleTexture);

		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load();
		setSamplerSate("samplerState", samplerState);

		// 4. Mesh
		DirectX::BoundingBox aabb{ { -10.0f, 0.0f, -10.0f },{ 10.0f, 20.0f, 10.0f } };
		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::Draw;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::PointList;
		drawCall.indexCount = 0;
		drawCall.vertexCount = m_maxParticlesCount;

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, nullptr, nullptr);
		mesh->load();
		setMesh(mesh);

		// 5. Render States
		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE;
		renderStates->getDepthStencilStateDesc()->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; 
		
		// Alpha Blending
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendEnable = true;
		renderStates->getBlendStateDesc()->RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderStates->getBlendStateDesc()->RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		renderStates->getBlendStateDesc()->RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		renderStates->getBlendStateDesc()->RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		renderStates->getBlendStateDesc()->RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Transparent);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		setRenderPass(renderPass);
		
		updateConstants();
	}

	void ParticleRainComponent::onUpdate(float deltaTime)
	{
		m_totalTime += deltaTime;
		m_particleUpdateCS->setConstant("deltaTime", deltaTime);
		m_particleUpdateCS->setConstant("totalTime", m_totalTime);
		
		if (auto node = m_node.lock())
		{
			m_particleUpdateCS->setConstant("emitterPosition", node->get_translation());
		}
		
		updateConstants();
	}

	void ParticleRainComponent::updateConstants()
	{
		m_particleUpdateCS->setConstant("rainAreaSize", m_rainAreaSize);
		m_particleUpdateCS->setConstant("fallSpeed", m_fallSpeed);
		m_particleUpdateCS->setConstant("dropLength", m_dropLength);
	}

	void ParticleRainComponent::onEnterScene()
	{
		VisualComponent::onEnterScene();
		if (auto node = m_node.lock())
		{
			m_particleUpdateCS->setConstant("emitterPosition", node->get_translation());
		}
		updateConstants();
	}

	void ParticleRainComponent::onPropertyChanged(const std::string& property)
	{
	}
	
	void ParticleRainComponent::set_maxParticlesCount(unsigned int maxParticlesCount)
	{
		m_maxParticlesCount = maxParticlesCount;
		initResources();
	}

	void ParticleRainComponent::set_rainAreaSize(DirectX::XMFLOAT3 rainAreaSize) { m_rainAreaSize = rainAreaSize; }
	void ParticleRainComponent::set_fallSpeed(float fallSpeed) { m_fallSpeed = fallSpeed; }
	void ParticleRainComponent::set_dropLength(float dropLength) { m_dropLength = dropLength; }

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ParticleRainComponent>("ParticleRainComponent")
			.constructor<>()
			.property("maxParticlesCount", &ParticleRainComponent::get_maxParticlesCount, &ParticleRainComponent::set_maxParticlesCount)
			.property("rainAreaSize", &ParticleRainComponent::get_rainAreaSize, &ParticleRainComponent::set_rainAreaSize)
			.property("fallSpeed", &ParticleRainComponent::get_fallSpeed, &ParticleRainComponent::set_fallSpeed)
			.property("dropLength", &ParticleRainComponent::get_dropLength, &ParticleRainComponent::set_dropLength);
	}
}
