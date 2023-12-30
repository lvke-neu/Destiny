#include "SkyboxComponent.h"
#include "Engine/Blob.h"
#include "Graphics/Visual3D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"

namespace Destiny
{
	using namespace DirectX;

	SkyboxComponent::SkyboxComponent()
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
		auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/Skybox_VS.cso");
		inputLayout->load(0);
		m_visual3D->setInputLayout(inputLayout);

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/Skybox_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/Skybox_PS.cso");
		pixelShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);

		data.reset(new Blob(sizeof(D3D11_RASTERIZER_DESC)));
		m_rasterizerStateDesc.CullMode = D3D11_CULL_FRONT;
		memcpy_s(data->getData(), data->getLength(), &m_rasterizerStateDesc, data->getLength());
		auto rasterizerState = Engine::GetInstance()->getGraphicsSystem()->createRasterizerState(data);
		rasterizerState->load(0);
		m_visual3D->setRasterizerState(rasterizerState);

		data.reset(new Blob(sizeof(D3D11_DEPTH_STENCIL_DESC)));
		m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		memcpy_s(data->getData(), data->getLength(), &m_depthStencilStateDesc, data->getLength());
		auto depthStencilState = Engine::GetInstance()->getGraphicsSystem()->createDepthStencilState(data);
		depthStencilState->load(0);
		m_visual3D->setDepthStencilState(depthStencilState);

		data.reset(new Blob(sizeof(D3D11_SAMPLER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &SamplerState::Default_SamplerState_Desc, data->getLength());
		m_cubeSamplerState = Engine::GetInstance()->getGraphicsSystem()->createSamplerState(data);
		m_cubeSamplerState->load(0);

		m_path = "assets://Texture/skybox/daylight.dds";
		m_cubeTexture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_path.c_str());
		m_cubeTexture->load();

		m_visual3D->registerBeforeDrawCommands(std::bind(&SkyboxComponent::beforeDrawCommand, this));
		m_visual3D->setRenderToMask(Visual3D::RenderToMask(Visual3D::render_to_scene | Visual3D::render_to_texture));
	}


	void SkyboxComponent::beforeDrawCommand()
	{
		if (!m_cubeTexture || !m_cubeTexture->isLoadingSucceed() ||
			!m_cubeSamplerState || !m_cubeSamplerState->isLoadingSucceed())
		{
			return;
		}
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		immediateContext->PSSetSamplers(0, 1, m_cubeSamplerState->getSamplerState());
		immediateContext->PSSetShaderResources(0, 1, m_cubeTexture->getShaderResourceView());
	}

	void SkyboxComponent::set_path(std::string path)
	{
		m_path = path;
		m_cubeTexture.reset();
		m_cubeTexture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_path.c_str());
		m_cubeTexture->load();
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<SkyboxComponent>("SkyboxComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			).property("path", &SkyboxComponent::get_path, &SkyboxComponent::set_path);
	}
}