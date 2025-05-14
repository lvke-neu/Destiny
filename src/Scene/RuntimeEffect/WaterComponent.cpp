#include "WaterComponent.h"
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
	WaterComponent::WaterComponent() :
		m_width(10.0f),
		m_depth(10.0f),
		m_m(50),
		m_n(50),
		m_sumTime(0.0f),
		m_waveParams1({ 0.5f, 0.5f, 0.7f, 1.0f }),
		m_waveParams2({ 0.2f, 1.2f, 0.4f, 1.0f }),
		m_waveParams3({ 0.1f, 2.0f, 0.9f, 1.0f }),
		m_waveColor({ 0.0f, 0.2f, 0.4f, 0.6f }),
		m_waveSpeed(1.0f),
		m_waveScale(20.0f),
		m_waveLevel(0.0f),
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_clearRenderTarget(std::make_shared<ClearRenderTarget>())
	{
		auto renderer = Renderer::Create("builtin://renderer/water.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = RenderStates::CreateBlendState();
		renderStates->getRasterizerStateDesc()->CullMode = D3D11_CULL_NONE;
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::Transparent);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
		set_material(PbrMaterial::Create_Water());

		setConstant("u_waveParams1", m_waveParams1);
		setConstant("u_waveParams2", m_waveParams2);
		setConstant("u_waveParams3", m_waveParams3);
		setConstant("u_waterColor", m_waveColor);
		setConstant("u_waveScale", m_waveScale);
		setConstant("u_waterLevel", m_waveLevel);

		//auto texture = Texture::Create("builtin://texture/brick.dds");
		//texture->load();
		//setShaderResource("t_underwater", texture);

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&WaterComponent::onResize, this, std::placeholders::_1));
		set_updateCategory(UpdateCategory::late_update);
	}

	WaterComponent::~WaterComponent()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&WaterComponent::onResize, this, std::placeholders::_1));
	}

	void WaterComponent::set_width(float width)
	{
		if (width <= 0)
		{
			return;
		}

		m_width = width;
	
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();
		setMesh(mesh);
	}

	void WaterComponent::set_depth(float depth)
	{
		if (depth <= 0)
		{
			return;
		}

		m_depth = depth;
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();
		setMesh(mesh);
	}

	void WaterComponent::set_m(unsigned int m)
	{
		if (m == 0)
		{
			return;
		}

		m_m = m;
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();
		setMesh(mesh);
	}

	void WaterComponent::set_n(unsigned int n)
	{
		if (n == 0)
		{
			return;
		}

		m_n = n;
		auto mesh = MeshProvider::Create_Water(m_width, m_depth, m_m, m_n);
		mesh->load();
		setMesh(mesh);
	}

	void WaterComponent::set_waveParams1(DirectX::XMFLOAT4 waveParams1)
	{
		m_waveParams1 = waveParams1;
		setConstant("u_waveParams1", m_waveParams1);
	}

	void WaterComponent::set_waveParams2(DirectX::XMFLOAT4 waveParams2)
	{
		m_waveParams2 = waveParams2;
		setConstant("u_waveParams2", m_waveParams2);
	}

	void WaterComponent::set_waveParams3(DirectX::XMFLOAT4 waveParams3)
	{
		m_waveParams3 = waveParams3;
		setConstant("u_waveParams3", m_waveParams3);
	}

	void WaterComponent::set_waveColor(Color waveColor)
	{
		m_waveColor = waveColor;
		setConstant("u_waterColor", m_waveColor);
	}

	void WaterComponent::set_waveScale(float waveScale)
	{
		m_waveScale = waveScale;
		setConstant("u_waveScale", m_waveScale);
	}

	void WaterComponent::set_waveLevel(float waveLevel)
	{
		m_waveLevel = waveLevel;
		setConstant("u_waterLevel", m_waveLevel);
	}

	void WaterComponent::onUpdate(float deltaTime)
	{
		m_sumTime += deltaTime * m_waveSpeed;
		setConstant("u_time", m_sumTime);


		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto forwardOpaquePipeline = std::static_pointer_cast<ForwardOpaquePipeline>(renderSystem->getForwardOpaquePipeline());

		renderSystem->beginEvent(L"Under Water");
		m_bindRenderTargets->execute(renderSystem->getImmediateContext());
		m_clearRenderTarget->execute(renderSystem->getImmediateContext());
		forwardOpaquePipeline->GraphicsCommandList::execute(renderSystem->getImmediateContext());
		renderSystem->endEvent();
	}

	void WaterComponent::onEnterScene()
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

	void WaterComponent::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}
		//DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
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
		rttr::registration::class_<WaterComponent>("WaterComponent")
			.constructor<>()
			.property("width", &WaterComponent::get_width, &WaterComponent::set_width)
			.property("depth", &WaterComponent::get_depth, &WaterComponent::set_depth)
			.property("m", &WaterComponent::get_m, &WaterComponent::set_m)
			.property("n", &WaterComponent::get_n, &WaterComponent::set_n)
			.property("waveParams1", &WaterComponent::get_waveParams1, &WaterComponent::set_waveParams1)
			.property("waveParams2", &WaterComponent::get_waveParams2, &WaterComponent::set_waveParams2)
			.property("waveParams3", &WaterComponent::get_waveParams2, &WaterComponent::set_waveParams2)
			.property("waveColor", &WaterComponent::get_waveColor, &WaterComponent::set_waveColor)
			.property("waveSpeed", &WaterComponent::get_waveSpeed, &WaterComponent::set_waveSpeed)
			.property("waveScale", &WaterComponent::get_waveScale, &WaterComponent::set_waveScale)
			.property("waveLevel", &WaterComponent::get_waveLevel, &WaterComponent::set_waveLevel);
	}
}