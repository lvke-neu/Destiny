#include "ImagePass.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "GraphicsSystem.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;

	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
		static D3D11_INPUT_ELEMENT_DESC inputLayout[3]; 
	};

	D3D11_INPUT_ELEMENT_DESC VertexPosColor::inputLayout[3] = 
	{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	struct ConstantData
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX invTranspose;
	};

	ImagePass::ImagePass()
	{
		VertexPosColor vertices[24];
		vertices[0].pos = XMFLOAT3(1, -1, -1);
		vertices[1].pos = XMFLOAT3(1, 1, -1);
		vertices[2].pos = XMFLOAT3(1, 1, 1);
		vertices[3].pos = XMFLOAT3(1, -1, 1);
		vertices[4].pos = XMFLOAT3(-1, -1, 1);
		vertices[5].pos = XMFLOAT3(-1, 1, 1);
		vertices[6].pos = XMFLOAT3(-1, 1, -1);
		vertices[7].pos = XMFLOAT3(-1, -1, -1);
		vertices[8].pos = XMFLOAT3(-1, 1, -1);
		vertices[9].pos = XMFLOAT3(-1, 1, 1);
		vertices[10].pos = XMFLOAT3(1, 1, 1);
		vertices[11].pos = XMFLOAT3(1, 1, -1);
		vertices[12].pos = XMFLOAT3(1, -1, -1);
		vertices[13].pos = XMFLOAT3(1, -1, 1);
		vertices[14].pos = XMFLOAT3(-1, -1, 1);
		vertices[15].pos = XMFLOAT3(-1, -1, -1);
		vertices[16].pos = XMFLOAT3(1, -1, 1);
		vertices[17].pos = XMFLOAT3(1, 1, 1);
		vertices[18].pos = XMFLOAT3(-1, 1, 1);
		vertices[19].pos = XMFLOAT3(-1, -1, 1);
		vertices[20].pos = XMFLOAT3(-1, -1, -1);
		vertices[21].pos = XMFLOAT3(-1, 1, -1);
		vertices[22].pos = XMFLOAT3(1, 1, -1);
		vertices[23].pos = XMFLOAT3(1, -1, -1);
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
			vertices[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
			vertices[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
			vertices[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
			vertices[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
		}
		DWORD indices[36] = {
			0, 1, 2, 2, 3, 0,		// 右面(+X面)
			4, 5, 6, 6, 7, 4,		// 左面(-X面)
			8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
			12, 13, 14, 14, 15, 12,	// 底面(-Y面)
			16, 17, 18, 18, 19, 16, // 背面(+Z面)
			20, 21, 22, 22, 23, 20	// 正面(-Z面)
		};

		auto device = Engine::GetInstance()->getGraphicsSystem()->getDevice();
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();

		m_vertexShader = std::make_unique<VertexShader>(device, L"HLSL/Phong_VS.cso", VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout));
		m_pixelShader = std::make_unique<PixelShader>(device, L"HLSL/Phong_PS.cso");
		m_vertexBuffer = std::make_unique<VertexBuffer>(device, sizeof(VertexPosColor), 0, vertices, sizeof(vertices));
		m_indexBuffer = std::make_unique<IndexBuffer>(device, DXGI_FORMAT_R32_UINT, indices, sizeof(indices));
		m_constantBuffer = std::make_unique<ConstantBuffer>(device, sizeof(ConstantData));
		m_texture = std::make_unique<Texture>(device, L"Texture/brick.dds");
		m_camera = std::make_unique<Camera>(device, immediateContext);

		Transform trans{ {10,10,1},{0,0,0}, {0,0,0} };
		ConstantData cd;
		cd.world = XMMatrixTranspose(trans.getWorldMatrix());
		XMMATRIX A = cd.world;
		A.r[3] = g_XMIdentityR3;

		cd.invTranspose = XMMatrixTranspose(XMMatrixTranspose(XMMatrixInverse(nullptr, A)));
		m_constantBuffer->updateData(immediateContext, &cd, sizeof(cd));
		

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		device->CreateSamplerState(&sampDesc, &m_pSamplerState);

		immediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer->m_pVertexBuffer, &m_vertexBuffer->m_stride, &m_vertexBuffer->m_offset);
		immediateContext->IASetInputLayout(m_vertexShader->m_pInputLayout);
		immediateContext->IASetIndexBuffer(m_indexBuffer->m_pIndexBuffer, m_indexBuffer->m_format, 0);

		immediateContext->VSSetShader(m_vertexShader->m_pVertexShader, nullptr, 0);
		immediateContext->PSSetShader(m_pixelShader->m_pPixelShader, nullptr, 0);
		immediateContext->PSSetShaderResources(0, 1, &m_texture->m_pShaderResourceView);
		immediateContext->PSSetSamplers(0, 1, &m_pSamplerState);
		immediateContext->VSSetConstantBuffers(2, 1, &m_constantBuffer->m_pConstantBuffer);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	ImagePass::~ImagePass()
	{
		SAFE_RELEASE(m_pSamplerState);
	}


	void ImagePass::draw()
	{
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		immediateContext->DrawIndexed(36, 0, 0);
	}
}