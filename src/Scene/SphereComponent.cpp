#include "SphereComponent.h"
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

namespace Destiny
{
	using namespace DirectX;

	SphereComponent::SphereComponent() :
		m_texturePath("assets://Texture/brick.dds")
	{
		std::shared_ptr<Blob> data = nullptr;
		struct VertexPosColor
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};

		const float radius = 1.0f;
		const UINT levels = 20;
		const UINT slices = 20;

		const UINT vertexCount = 2 + (levels - 1) * (slices + 1);
		VertexPosColor vertices[vertexCount];

		DWORD vIndex = 0, iIndex = 0;

		float phi = 0.0f, theta = 0.0f;
		float per_phi = XM_PI / levels;
		float per_theta = XM_2PI / slices;
		float x, y, z;

		vertices[vIndex++] = VertexPosColor({ XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });

		for (UINT i = 1; i < levels; ++i)
		{
			phi = per_phi * i;

			for (UINT j = 0; j <= slices; ++j)
			{
				theta = per_theta * j;
				x = radius * sinf(phi) * cosf(theta);
				y = radius * cosf(phi);
				z = radius * sinf(phi) * sinf(theta);

				XMFLOAT3 pos = XMFLOAT3(x, y, z), normal;
				XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

				vertices[vIndex++] = VertexPosColor({ pos, normal, XMFLOAT2(theta / XM_2PI, phi / XM_PI) });
			}
		}
		vertices[vIndex++] = VertexPosColor({ XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) });
		
		data.reset(new Blob(vertexCount * sizeof(VertexPosColor)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosColor), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		unsigned int indices[6 * (levels - 1) * slices];
		if (levels > 1)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = 0;
				indices[iIndex++] = j % (slices + 1) + 1;
				indices[iIndex++] = j;
			}
		}
		for (UINT i = 1; i < levels - 1; ++i)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = (i - 1) * (slices + 1) + j;
				indices[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

				indices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = i * (slices + 1) + j;
				indices[iIndex++] = (i - 1) * (slices + 1) + j;
			}
		}
		if (levels > 1)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = (levels - 2) * (slices + 1) + j;
				indices[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = (levels - 1) * (slices + 1) + 1;
			}
		}

		data.reset(new Blob(sizeof(unsigned int) * 6 * (levels - 1) * slices));
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

		m_texture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_texturePath.c_str());
		m_texture->load();

		data.reset(new Blob(sizeof(D3D11_SAMPLER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &SamplerState::Default_SamplerState_Desc, data->getLength());
		m_samplerState = Engine::GetInstance()->getGraphicsSystem()->createSamplerState(data);
		m_samplerState->load(0);

		m_visual3D->registerBeforeDrawCommands(std::bind(&SphereComponent::beforeDrawCommands, this));
	}

	void SphereComponent::set_texturePath(std::string texturePath)
	{
		m_texturePath = texturePath;
		m_texture.reset();
		m_texture = Engine::GetInstance()->getGraphicsSystem()->createTexture(m_texturePath.c_str());
		m_texture->load();
	}

	void SphereComponent::beforeDrawCommands()
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
		rttr::registration::class_<SphereComponent>("SphereComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
		    .property("texturePath", &SphereComponent::get_texturePath, &SphereComponent::set_texturePath);
	}
}