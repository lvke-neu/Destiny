#include "BoxComponent.h"
#include "Engine/Blob.h"
#include "Graphics/Visual3D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/RasterizerState.h"
#include "Graphics/DepthStencilState.h"
#include "Graphics/BlendState.h"
#include "Graphics/Texture.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/SamplerState.h"
#include <DirectXMath.h>

namespace Destiny
{
	static std::shared_ptr<ConstantBuffer<DirectX::XMMATRIX>> cbWorld = nullptr;
	static std::shared_ptr<ConstantBuffer<DirectX::XMMATRIX>> cbView = nullptr;
	static std::shared_ptr<ConstantBuffer<DirectX::XMMATRIX>> cbProj = nullptr;
	static std::shared_ptr<Texture> tex = nullptr;
	static std::shared_ptr<SamplerState> samplerState = nullptr;
	BoxComponent::BoxComponent()
	{
		std::shared_ptr<Blob> data = nullptr;
		using namespace DirectX;
		struct VertexPosColor
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};
		VertexPosColor vertices[24];
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
		data.reset(new Blob(24 * sizeof(VertexPosColor)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosColor), 0, data);
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

		D3D11_RASTERIZER_DESC rasterizerDesc = RasterizerState::Default_Rasterizer_Desc;
		data.reset(new Blob(sizeof(D3D11_RASTERIZER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &rasterizerDesc, data->getLength());
		auto rasterizerState = Engine::GetInstance()->getGraphicsSystem()->createRasterizerState(data);
		rasterizerState->load(0);
		m_visual3D->setRasterizerState(rasterizerState);

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = DepthStencilState::Default_DepthStencil_Desc;
		data.reset(new Blob(sizeof(D3D11_DEPTH_STENCIL_DESC)));
		memcpy_s(data->getData(), data->getLength(), &depthStencilStateDesc, data->getLength());
		auto depthStencilState = Engine::GetInstance()->getGraphicsSystem()->createDepthStencilState(data);
		depthStencilState->load(0);
		m_visual3D->setDepthStencilState(depthStencilState);

		D3D11_BLEND_DESC blendStateDesc = BlendState::Default_BlendState_Desc;
		data.reset(new Blob(sizeof(D3D11_BLEND_DESC)));
		memcpy_s(data->getData(), data->getLength(), &blendStateDesc, data->getLength());
		auto blendState = Engine::GetInstance()->getGraphicsSystem()->createBlendState(data);
		blendState->load(0);
		m_visual3D->setBlendState(blendState);

		tex = Engine::GetInstance()->getGraphicsSystem()->createTexture("assets://Texture/brick.dds");
		tex->load();

		data.reset(new Blob(sizeof(D3D11_SAMPLER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &SamplerState::Default_SamplerState_Desc, data->getLength());
		samplerState = Engine::GetInstance()->getGraphicsSystem()->createSamplerState(data);
		samplerState->load(0);

		cbWorld = std::make_shared<ConstantBuffer<XMMATRIX>>();
		cbWorld->update(XMMatrixTranspose(XMMatrixTranslation(0.0f, 0.0f, 5.0f)));

		cbView = std::make_shared<ConstantBuffer<XMMATRIX>>();
		cbView->update(XMMatrixTranspose(XMMatrixIdentity()));

		cbProj = std::make_shared<ConstantBuffer<DirectX::XMMATRIX>>();
		cbProj->update(XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, 438.0f / 600.0f, 1.0f, 1000.0f)));

		m_visual3D->setBeforeDrawCommands(std::bind(&BoxComponent::beforeDrawCommands, this));
	}

	void BoxComponent::beforeDrawCommands()
	{
		if (
			!samplerState || !samplerState->isLoadingSucceed() ||
			!tex || !tex->isLoadingSucceed()
			)
		{
			return;
		}

		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(0, 1, cbView->getConstantBuffer());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(1, 1, cbProj->getConstantBuffer());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(2, 1, cbWorld->getConstantBuffer());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetSamplers(0, 1, samplerState->getSamplerState());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetShaderResources(0, 1, tex->getShaderResourceView());

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