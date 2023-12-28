#include "BoxComponent.h"
#include "Engine/Blob.h"
#include "Graphics/Visual3D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Material.h"
#include "Graphics/RenderTargetView.h"
#include "Graphics/DepthStencilView.h"
#include "Engine/EventSystem.h"

namespace Destiny
{
	using namespace DirectX;

	BoxComponent::BoxComponent()
	{
		std::shared_ptr<Blob> data = nullptr;
		struct VertexPosTexcoord
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};
		VertexPosTexcoord vertices[24];
		vertices[0].position = XMFLOAT3(1, -1, -1);
		vertices[1].position = XMFLOAT3(1, 1, -1);
		vertices[2].position = XMFLOAT3(1, 1, 1);
		vertices[3].position = XMFLOAT3(1, -1, 1);
		vertices[4].position = XMFLOAT3(-1, -1, 1);
		vertices[5].position = XMFLOAT3(-1, 1, 1);
		vertices[6].position = XMFLOAT3(-1, 1, -1);
		vertices[7].position = XMFLOAT3(-1, -1, -1);
		vertices[8].position = XMFLOAT3(-1, 1, -1);
		vertices[9].position = XMFLOAT3(-1, 1, 1);
		vertices[10].position = XMFLOAT3(1, 1, 1);
		vertices[11].position = XMFLOAT3(1, 1, -1);
		vertices[12].position = XMFLOAT3(1, -1, -1);
		vertices[13].position = XMFLOAT3(1, -1, 1);
		vertices[14].position = XMFLOAT3(-1, -1, 1);
		vertices[15].position = XMFLOAT3(-1, -1, -1);
		vertices[16].position = XMFLOAT3(1, -1, 1);
		vertices[17].position = XMFLOAT3(1, 1, 1);
		vertices[18].position = XMFLOAT3(-1, 1, 1);
		vertices[19].position = XMFLOAT3(-1, -1, 1);
		vertices[20].position = XMFLOAT3(-1, -1, -1);
		vertices[21].position = XMFLOAT3(-1, 1, -1);
		vertices[22].position = XMFLOAT3(1, 1, -1);
		vertices[23].position = XMFLOAT3(1, -1, -1);
		for (UINT i = 0; i < 4; ++i)
		{
			vertices[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertices[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertices[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		}
		for (UINT i = 0; i < 6; ++i)
		{
			vertices[i * 4].texcoord = XMFLOAT2(0.0f, 1.0f);
			vertices[i * 4 + 1].texcoord = XMFLOAT2(0.0f, 0.0f);
			vertices[i * 4 + 2].texcoord = XMFLOAT2(1.0f, 0.0f);
			vertices[i * 4 + 3].texcoord = XMFLOAT2(1.0f, 1.0f);
		}
		data.reset(new Blob(24 * sizeof(VertexPosTexcoord)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosTexcoord), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		unsigned int indices[36] = {
						0, 1, 2, 2, 3, 0,
						4, 5, 6, 6, 7, 4,
						8, 9, 10, 10, 11, 8,
						12, 13, 14, 14, 15, 12,
						16, 17, 18, 18, 19, 16,
						20, 21, 22, 22, 23, 20
		};
		data.reset(new Blob(sizeof(unsigned int) * 36));
		memcpy_s(data->getData(), data->getLength(), indices, data->getLength());
		auto indexBuffer = Engine::GetInstance()->getGraphicsSystem()->createIndexBuffer(DXGI_FORMAT_R32_UINT, data);
		indexBuffer->load(0);
		m_visual3D->setIndexBuffer(indexBuffer);


		D3D11_INPUT_ELEMENT_DESC inputElements[3] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		data.reset(new Blob(3 * sizeof(D3D11_INPUT_ELEMENT_DESC)));
		memcpy_s(data->getData(), data->getLength(), inputElements, data->getLength());
		auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/Basic_VS.cso");
		inputLayout->load(0);
		m_visual3D->setInputLayout(inputLayout);

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/Basic_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/Basic_PS.cso");
		pixelShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);

		m_material->set_useColor(false);
		m_material->set_ambientTexturePath("assets://Texture/box_diffuse.png");
		m_material->set_diffuseTexturePath("assets://Texture/box_diffuse.png");
		m_material->set_specularTexturePath("assets://Texture/box_specular.png");
		m_material->load();

		m_visual3D->registerBeforeDrawCommands(std::bind(&BoxComponent::beforeDrawCommand, this));
		m_visual3D->registerAfterDrawCommands(std::bind(&BoxComponent::afterDrawCommand, this));

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&BoxComponent::onResize, this, std::placeholders::_1));
	}

	void BoxComponent::beforeDrawCommand()
	{
		if (!m_renderTargetView || !m_renderTargetView->isLoadingSucceed() ||
			!m_depthStencilView || !m_depthStencilView->isLoadingSucceed())
		{
			return;
		}

		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		immediateContext->ClearRenderTargetView(*m_renderTargetView->getRenderTargetView(), Color::White.toFloat());
		immediateContext->ClearDepthStencilView(m_depthStencilView->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		immediateContext->OMSetRenderTargets(1, m_renderTargetView->getRenderTargetView(), m_depthStencilView->getDepthStencilView());
	}

	void BoxComponent::afterDrawCommand()
	{
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		ID3D11RenderTargetView* pRTV = nullptr;
		immediateContext->OMSetRenderTargets(1, &pRTV, nullptr);
	}

	BoxComponent::~BoxComponent()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&BoxComponent::onResize, this, std::placeholders::_1));
	}

	void BoxComponent::onResize(void* data)
	{
		WindowResizeData windowResizeData = *(WindowResizeData*)data;

		m_renderTargetView.reset();
		m_depthStencilView.reset();

		m_renderTargetView = std::make_shared<RenderTargetView>(windowResizeData.width, windowResizeData.height);
		m_renderTargetView->load(0);

		m_depthStencilView = std::make_shared<DepthStencilView>(windowResizeData.width, windowResizeData.height);
		m_depthStencilView->load(0);
	}
	RTTR_REGISTRATION
	{
		rttr::registration::class_<BoxComponent>("BoxComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}