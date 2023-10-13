#include "ImagePass.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;

	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		static D3D11_INPUT_ELEMENT_DESC inputLayout[2]; 
	};

	D3D11_INPUT_ELEMENT_DESC VertexPosColor::inputLayout[2] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	struct ConstantData
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	ImagePass::ImagePass()
	{
		VertexPosColor vertices[] =
		{
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
		};

		DWORD indices[] = {
			// 正面
			0, 1, 2,
			2, 3, 0,
			// 左面
			4, 5, 1,
			1, 0, 4,
			// 顶面
			1, 5, 6,
			6, 2, 1,
			// 背面
			7, 6, 5,
			5, 4, 7,
			// 右面
			3, 2, 6,
			6, 7, 3,
			// 底面
			4, 0, 3,
			3, 7, 4
		};

		auto device = Engine::GetInstance()->getGraphicsSystem()->getDevice();

		m_vertexShader = std::make_unique<VertexShader>(device, L"HLSL/Phong_VS.cso", VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout));
		m_pixelShader = std::make_unique<PixelShader>(device, L"HLSL/Phong_PS.cso");
		m_vertexBuffer = std::make_unique<VertexBuffer>(device, sizeof(VertexPosColor), 0, vertices, sizeof(vertices));
		m_indexBuffer = std::make_unique<IndexBuffer>(device, DXGI_FORMAT_R32_UINT, indices, sizeof(indices));
		m_constantBuffer = std::make_unique<ConstantBuffer>(device, sizeof(ConstantData));
	}

	ImagePass::~ImagePass()
	{

	}

	void ImagePass::draw()
	{
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();

		immediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer->m_pVertexBuffer, &m_vertexBuffer->m_stride, &m_vertexBuffer->m_offset);
		immediateContext->IASetInputLayout(m_vertexShader->m_pInputLayout);
		immediateContext->IASetIndexBuffer(m_indexBuffer->m_pIndexBuffer, m_indexBuffer->m_format, 0);
		
		immediateContext->VSSetShader(m_vertexShader->m_pVertexShader, nullptr, 0);
		immediateContext->PSSetShader(m_pixelShader->m_pPixelShader, nullptr, 0);
		immediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer->m_pConstantBuffer);


		ConstantData cd;
		static float phi = 0.0f, theta = 0.0f;
		phi += 0.0001f, theta += 0.00015f;
		cd.world = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));
		cd.view = XMMatrixTranspose(XMMatrixLookAtLH(
			XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
		cd.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 1.0f, 1000.0f));

		m_constantBuffer->updateData(immediateContext, &cd, sizeof(cd));

		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		immediateContext->DrawIndexed(36, 0, 0);
	}
}