#include "BoxComponent.h"
#include "Engine/Blob.h"
#include "Graphics/Visual3D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"

namespace Destiny
{
	using namespace DirectX;

	BoxComponent::BoxComponent() :
		m_texturePath("assets://Texture/brick.dds")
	{
		std::shared_ptr<Blob> data = nullptr;
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

		m_texture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_texturePath.c_str());
		m_texture->load();

		data.reset(new Blob(sizeof(D3D11_SAMPLER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &SamplerState::Default_SamplerState_Desc, data->getLength());
		m_samplerState = Engine::GetInstance()->getGraphicsSystem()->createSamplerState(data);
		m_samplerState->load(0);

		m_visual3D->registerBeforeDrawCommands(std::bind(&BoxComponent::beforeDrawCommands, this));
	}

	void BoxComponent::set_texturePath(std::string texturePath)
	{
		m_texturePath = texturePath;
		m_texture.reset();
		m_texture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_texturePath.c_str());
		m_texture->load();
	}

	void BoxComponent::beforeDrawCommands()
	{
		if (
			!m_samplerState || !m_samplerState->isLoadingSucceed() ||
			!m_texture || !m_texture->isLoadingSucceed()
			)
		{
			return;
		}
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetSamplers(0, 1, m_samplerState->getSamplerState());
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetShaderResources(0, 1, m_texture->getShaderResourceView());
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BoxComponent>("BoxComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
		    .property("texturePath", &BoxComponent::get_texturePath, &BoxComponent::set_texturePath);
	}
}