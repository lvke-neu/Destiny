#include "RenderToTextureComponent.h"
#include "Engine/Blob.h"
#include "Engine/EventSystem.h"
#include "Graphics/Visual3D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderTargetView.h"
#include "Graphics/DepthStencilView.h"
#include "Graphics/SamplerState.h"
#include "Graphics/GraphicsSystem.h"

namespace Destiny
{
	using namespace DirectX;

	RenderToTextureComponent::RenderToTextureComponent()
	{
		std::shared_ptr<Blob> data = nullptr;
		struct VertexPosTexcoord
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};
		VertexPosTexcoord vertices[4];
		vertices[0].position = XMFLOAT3(0.5f, -0.5f, 0.0f);
		vertices[1].position = XMFLOAT3(1.0f, -0.5f, 0.0f);
		vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertices[3].position = XMFLOAT3(0.5f, -1.0f, 0.0f);
		vertices[0].texcoord = XMFLOAT2(0.0f, 0.0f);
		vertices[1].texcoord = XMFLOAT2(1.0f, 0.0f);
		vertices[2].texcoord = XMFLOAT2(1.0f, 1.0f);
		vertices[3].texcoord = XMFLOAT2(0.0f, 1.0f);

		data.reset(new Blob(4 * sizeof(VertexPosTexcoord)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosTexcoord), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		unsigned int indices[6] = { 0, 1, 3, 1, 2, 3 };
		data.reset(new Blob(sizeof(unsigned int) * 6));
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
		auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/RenderToTexture_VS.cso");
		inputLayout->load(0);
		m_visual3D->setInputLayout(inputLayout);

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/RenderToTexture_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/RenderToTexture_PS.cso");
		pixelShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);

		data.reset(new Blob(sizeof(D3D11_SAMPLER_DESC)));
		D3D11_SAMPLER_DESC samplerDesc = SamplerState::Default_SamplerState_Desc;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		memcpy_s(data->getData(), data->getLength(), &samplerDesc, data->getLength());
		m_samplerState = Engine::GetInstance()->getGraphicsSystem()->createSamplerState(data);
		m_samplerState->load(0);

		m_visual3D->registerBeforeDrawCommands(std::bind(&RenderToTextureComponent::beforeDrawCommand, this));
	}

	void RenderToTextureComponent::beforeDrawCommand()
	{
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		immediateContext->PSSetSamplers(0, 1, m_samplerState->getSamplerState());
		immediateContext->PSSetShaderResources(0, 1, Engine::GetInstance()->getGraphicsSystem()->getRenderToShadowMapDSV()->getShaderResourceView());
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<RenderToTextureComponent>("RenderToTextureComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}