#include "LakeRiverComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/Texture.h"
#include "Graphics/VisualScene.h"
#include "Graphics/ForwardOpaquePipeline.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/BindRenderTargets.h"
#include "Graphics/ClearRenderTarget.h"
#include "Graphics/RenderTargetView.h"
#include "Graphics/DepthStencilView.h"
#include "Engine/EventSystem.h"

namespace Destiny
{
	LakeRiverComponent::LakeRiverComponent() :
		m_width(20.0f),
		m_depth(20.0f),
		m_m(100),
		m_n(100),
		m_sumTime(0.0f),
		m_waveParams1({ 0.2f, 0.5f, 0.5f, 1.0f }),
		m_waveParams2({ 0.1f, 1.0f, 0.3f, 1.0f }),
		m_waveParams3({ 0.05f, 1.5f, 0.7f, 1.0f }),
		m_waveColor({ 0.0f, 0.3f, 0.5f, 0.7f }),
		m_waveScale(10.0f),
		m_waveLevel(0.0f),
		m_flowSpeed({ 0.05f, 0.05f }),
		m_flowCycleScale(0.2f),
		m_flowStrength(0.1f),
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_clearRenderTarget(std::make_shared<ClearRenderTarget>())
	{
		// 1. Create Renderer with new shader
		auto renderer = Renderer::Create("builtin://renderer/lake_river.hlsl");
		renderer->load(0);

		// 2. Set Render States
		std::shared_ptr<RenderStates> renderStates = RenderStates::CreateBlendState();
		renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE;
		renderStates->load();

		// 3. Create RenderPass
		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Transparent);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		// 4. Create Mesh
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();

		// 5. Setup Component
		setRenderPass(renderPass);
		setMesh(mesh);
		set_material(PbrMaterial::Create_Water()); // Use default water material for basic textures

		// 6. Set Shader Constants
		setConstant("u_waveParams1", m_waveParams1);
		setConstant("u_waveParams2", m_waveParams2);
		setConstant("u_waveParams3", m_waveParams3);
		setConstant("u_waterColor", m_waveColor);
		setConstant("u_waveScale", m_waveScale);
		setConstant("u_waterLevel", m_waveLevel);
		setConstant("u_flowSpeed", m_flowSpeed);
		setConstant("u_flowCycleScale", m_flowCycleScale);
		setConstant("u_flowStrength", m_flowStrength);

		// Default Flow Map (Use noise or flat normal if not provided, here we assume a default exists or user sets it)
		// For now, let's just use the normal map as flow map placeholder or a default noise texture if available.
		// A proper flow map should be set by the user. 
		// We can reuse the "brick" texture as a dummy flow map if needed, but better to let user assign it.
		// auto flowMap = Texture::Create("builtin://texture/noise.dds"); // Assuming noise exists
		// setShaderResource("t_flowMap", flowMap);

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&LakeRiverComponent::onResize, this, std::placeholders::_1));
		set_updateCategory(UpdateCategory::late_update);
	}

	LakeRiverComponent::~LakeRiverComponent()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&LakeRiverComponent::onResize, this, std::placeholders::_1));
	}

	void LakeRiverComponent::set_width(float width)
	{
		if (width <= 0) return;
		m_width = width;
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();
		setMesh(mesh);
	}

	void LakeRiverComponent::set_depth(float depth)
	{
		if (depth <= 0) return;
		m_depth = depth;
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();
		setMesh(mesh);
	}

	void LakeRiverComponent::set_m(unsigned int m)
	{
		if (m == 0) return;
		m_m = m;
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();
		setMesh(mesh);
	}

	void LakeRiverComponent::set_n(unsigned int n)
	{
		if (n == 0) return;
		m_n = n;
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();
		setMesh(mesh);
	}

	void LakeRiverComponent::set_waveParams1(DirectX::XMFLOAT4 waveParams1)
	{
		m_waveParams1 = waveParams1;
		setConstant("u_waveParams1", m_waveParams1);
	}

	void LakeRiverComponent::set_waveParams2(DirectX::XMFLOAT4 waveParams2)
	{
		m_waveParams2 = waveParams2;
		setConstant("u_waveParams2", m_waveParams2);
	}

	void LakeRiverComponent::set_waveParams3(DirectX::XMFLOAT4 waveParams3)
	{
		m_waveParams3 = waveParams3;
		setConstant("u_waveParams3", m_waveParams3);
	}

	void LakeRiverComponent::set_waveColor(Color waveColor)
	{
		m_waveColor = waveColor;
		setConstant("u_waterColor", m_waveColor);
	}

	void LakeRiverComponent::set_waveScale(float waveScale)
	{
		m_waveScale = waveScale;
		setConstant("u_waveScale", m_waveScale);
	}

	void LakeRiverComponent::set_waveLevel(float waveLevel)
	{
		m_waveLevel = waveLevel;
		setConstant("u_waterLevel", m_waveLevel);
	}

	void LakeRiverComponent::set_flowSpeed(DirectX::XMFLOAT2 flowSpeed)
	{
		m_flowSpeed = flowSpeed;
		setConstant("u_flowSpeed", m_flowSpeed);
	}

	void LakeRiverComponent::set_flowCycleScale(float flowCycleScale)
	{
		m_flowCycleScale = flowCycleScale;
		setConstant("u_flowCycleScale", m_flowCycleScale);
	}

	void LakeRiverComponent::set_flowStrength(float flowStrength)
	{
		m_flowStrength = flowStrength;
		setConstant("u_flowStrength", m_flowStrength);
	}

	void LakeRiverComponent::onUpdate(float deltaTime)
	{
		m_sumTime += deltaTime;
		setConstant("u_time", m_sumTime);

		// Render underwater scene to texture
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto forwardOpaquePipeline = std::static_pointer_cast<ForwardOpaquePipeline>(renderSystem->getForwardOpaquePipeline());

		renderSystem->beginEvent(L"Under Water");
		m_bindRenderTargets->execute(renderSystem->getImmediateContext());
		m_clearRenderTarget->execute(renderSystem->getImmediateContext());
		forwardOpaquePipeline->GraphicsCommandList::execute(renderSystem->getImmediateContext());
		renderSystem->endEvent();
	}

	void LakeRiverComponent::onEnterScene()
	{
		VisualComponent::onEnterScene();
		
		WindowResizeData wrd;
		wrd.width = 100;
		wrd.height = 100;
		
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		if (renderSystem)
		{
			if (renderSystem->m_bindRenderTargets->getRenderTargetViews(0))
			{
				wrd.width = renderSystem->m_bindRenderTargets->getRenderTargetViews(0)->getWidth();
				wrd.height = renderSystem->m_bindRenderTargets->getRenderTargetViews(0)->getHeight();
			}
		}
		
		onResize(&wrd);
	}

	void LakeRiverComponent::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}

		auto renderTargetView0 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView0->load(0);

		auto depthStencilView0 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView0->load(0);

		auto viewPort0 = std::make_shared<D3D11_VIEWPORT>();
		viewPort0->TopLeftX = 0.0f;
		viewPort0->TopLeftY = 0.0f;
		viewPort0->Width = (float)wrd.width;
		viewPort0->Height = (float)wrd.height;
		viewPort0->MinDepth = 0.0f;
		viewPort0->MaxDepth = 1.0f;

		std::vector<std::shared_ptr<RenderTargetView>>	renderTargetViews;
		renderTargetViews.push_back(renderTargetView0);

		std::vector<std::shared_ptr<DepthStencilView>>	depthStencilViews;
		depthStencilViews.push_back(depthStencilView0);

		std::vector<std::shared_ptr<D3D11_VIEWPORT>>	viewPorts;
		viewPorts.push_back(viewPort0);

		m_bindRenderTargets->setRenderTargetViews(renderTargetViews);
		m_bindRenderTargets->setDepthStencilViews(depthStencilViews);
		m_bindRenderTargets->setViewports(viewPorts);

		m_clearRenderTarget->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(0));
		m_clearRenderTarget->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(0));

		setShaderResource("t_underwater", renderTargetView0->getTexture());
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<LakeRiverComponent>("LakeRiverComponent")
			.constructor<>()
			.property("width", &LakeRiverComponent::get_width, &LakeRiverComponent::set_width)
			.property("depth", &LakeRiverComponent::get_depth, &LakeRiverComponent::set_depth)
			.property("m", &LakeRiverComponent::get_m, &LakeRiverComponent::set_m)
			.property("n", &LakeRiverComponent::get_n, &LakeRiverComponent::set_n)
			.property("waveParams1", &LakeRiverComponent::get_waveParams1, &LakeRiverComponent::set_waveParams1)
			.property("waveParams2", &LakeRiverComponent::get_waveParams2, &LakeRiverComponent::set_waveParams2)
			.property("waveParams3", &LakeRiverComponent::get_waveParams3, &LakeRiverComponent::set_waveParams3)
			.property("waveColor", &LakeRiverComponent::get_waveColor, &LakeRiverComponent::set_waveColor)
			.property("waveScale", &LakeRiverComponent::get_waveScale, &LakeRiverComponent::set_waveScale)
			.property("waveLevel", &LakeRiverComponent::get_waveLevel, &LakeRiverComponent::set_waveLevel)
			.property("flowSpeed", &LakeRiverComponent::get_flowSpeed, &LakeRiverComponent::set_flowSpeed)
			.property("flowCycleScale", &LakeRiverComponent::get_flowCycleScale, &LakeRiverComponent::set_flowCycleScale)
			.property("flowStrength", &LakeRiverComponent::get_flowStrength, &LakeRiverComponent::set_flowStrength);
	}
}
