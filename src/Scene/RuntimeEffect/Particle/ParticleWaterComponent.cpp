#include "ParticleWaterComponent.h"
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
	ParticleWaterComponent::ParticleWaterComponent() :
		m_maxParticlesCount(5000),
		m_totalTime(0.0f),
		m_gravity({ 0.0f, -12.0f, 0.0f }),
		m_spreadRadius(3.0f),
		m_splashHeight(4.0f),
		m_particleMinSize(0.1f),
		m_particleMaxSize(0.3f),
		m_particleMinLife(0.8f),
		m_particleMaxLife(1.4f),
		m_particleUpdateCS(std::make_shared<ComputerCommand>())
	{
		m_particleUpdateCS->setComputerEffectPath("builtin://renderer/particle_water_update.hlsl");
		m_particleUpdateCS->setDebugName(L"ParticleWaterUpdate");
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommand(m_particleUpdateCS);

		initResources();
	}

	ParticleWaterComponent::~ParticleWaterComponent()
	{
	}

	void ParticleWaterComponent::initResources()
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
		auto renderer = Renderer::Create("builtin://renderer/particle_water_render.hlsl");
		renderer->load(0);
		
		setShaderResource("particles", m_particlesBuffer);
		
		auto particleTexture = Texture::Create("builtin://texture/particle/foam_gen.dds"); 
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
		
		// Initial constant update
		updateConstants();
	}

	void ParticleWaterComponent::onUpdate(float deltaTime)
	{
		m_totalTime += deltaTime;
		m_particleUpdateCS->setConstant("deltaTime", deltaTime);
		m_particleUpdateCS->setConstant("totalTime", m_totalTime);
		
		if (auto node = m_node.lock())
		{
			m_particleUpdateCS->setConstant("emitterPosition", node->get_translation());
		}
		
		// Ensure constants are up to date every frame (or only on change if optimized)
		updateConstants();
	}

	void ParticleWaterComponent::updateConstants()
	{
		m_particleUpdateCS->setConstant("gravity", m_gravity);
		m_particleUpdateCS->setConstant("spreadRadius", m_spreadRadius);
		m_particleUpdateCS->setConstant("splashHeight", m_splashHeight);
		m_particleUpdateCS->setConstant("particleMinSize", m_particleMinSize);
		m_particleUpdateCS->setConstant("particleMaxSize", m_particleMaxSize);
		m_particleUpdateCS->setConstant("particleMinLife", m_particleMinLife);
		m_particleUpdateCS->setConstant("particleMaxLife", m_particleMaxLife);
	}

	void ParticleWaterComponent::onEnterScene()
	{
		VisualComponent::onEnterScene();
		if (auto node = m_node.lock())
		{
			m_particleUpdateCS->setConstant("emitterPosition", node->get_translation());
		}
		updateConstants();
	}

	void ParticleWaterComponent::onPropertyChanged(const std::string& property)
	{
		// Could optimize to only call updateConstants here
	}
	
	void ParticleWaterComponent::set_maxParticlesCount(unsigned int maxParticlesCount)
	{
		m_maxParticlesCount = maxParticlesCount;
		initResources();
	}

	// Setters
	void ParticleWaterComponent::set_gravity(DirectX::XMFLOAT3 gravity) { m_gravity = gravity; }
	void ParticleWaterComponent::set_spreadRadius(float spreadRadius) { m_spreadRadius = spreadRadius; }
	void ParticleWaterComponent::set_splashHeight(float splashHeight) { m_splashHeight = splashHeight; }
	void ParticleWaterComponent::set_particleMinSize(float particleMinSize) { m_particleMinSize = particleMinSize; }
	void ParticleWaterComponent::set_particleMaxSize(float particleMaxSize) { m_particleMaxSize = particleMaxSize; }
	void ParticleWaterComponent::set_particleMinLife(float particleMinLife) { m_particleMinLife = particleMinLife; }
	void ParticleWaterComponent::set_particleMaxLife(float particleMaxLife) { m_particleMaxLife = particleMaxLife; }

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ParticleWaterComponent>("ParticleWaterComponent")
			.constructor<>()
			.property("maxParticlesCount", &ParticleWaterComponent::get_maxParticlesCount, &ParticleWaterComponent::set_maxParticlesCount)
			.property("gravity", &ParticleWaterComponent::get_gravity, &ParticleWaterComponent::set_gravity)
			.property("spreadRadius", &ParticleWaterComponent::get_spreadRadius, &ParticleWaterComponent::set_spreadRadius)
			.property("splashHeight", &ParticleWaterComponent::get_splashHeight, &ParticleWaterComponent::set_splashHeight)
			.property("particleMinSize", &ParticleWaterComponent::get_particleMinSize, &ParticleWaterComponent::set_particleMinSize)
			.property("particleMaxSize", &ParticleWaterComponent::get_particleMaxSize, &ParticleWaterComponent::set_particleMaxSize)
			.property("particleMinLife", &ParticleWaterComponent::get_particleMinLife, &ParticleWaterComponent::set_particleMinLife)
			.property("particleMaxLife", &ParticleWaterComponent::get_particleMaxLife, &ParticleWaterComponent::set_particleMaxLife);
	}
}
