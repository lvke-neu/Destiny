#include "ImagePass.h"
#include "Engine/Engine.h"
#include "Engine/Utility.h"
#include "GraphicsSystem.h"
#include <d3d11.h>
#include <d3dcompiler.h>
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

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	ImagePass::ImagePass()
	{
		

		auto device = Engine::GetInstance()->getGraphicsSystem()->getDevice();

		ID3DBlob* blob{ nullptr };

		D3DReadFileToBlob(L"HLSL/Cube_VS.cso", &blob);
		device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pD3D11VertexShader);
		device->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout), blob->GetBufferPointer(), blob->GetBufferSize(), &m_pD3D11InputLayout);
		SAFE_RELEASE(blob);

		D3DReadFileToBlob(L"HLSL/Cube_PS.cso", &blob);
		device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pD3D11PixelShader);
		SAFE_RELEASE(blob);

		VertexPosColor vertices[] =
		{
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
		};
		// 设置顶点缓冲区描述
		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(vbd));
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof vertices;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		// 新建顶点缓冲区
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
		device->CreateBuffer(&vbd, &InitData, &m_pD3D11VertexBuffer);


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
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof indices;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		InitData.pSysMem = indices;
		device->CreateBuffer(&ibd, &InitData, &m_pD3D11IndexBuffer);

		D3D11_BUFFER_DESC cbd;
		ZeroMemory(&cbd, sizeof(cbd));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = sizeof(ConstantBuffer);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		device->CreateBuffer(&cbd, nullptr, &m_pD3D11ConstantBuffer);

		


		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		UINT stride = sizeof(VertexPosColor);	// 跨越字节数
		UINT offset = 0;						// 起始偏移量

		immediateContext->IASetVertexBuffers(0, 1, &m_pD3D11VertexBuffer, &stride, &offset);
		immediateContext->IASetInputLayout(m_pD3D11InputLayout);
		immediateContext->IASetIndexBuffer(m_pD3D11IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		immediateContext->VSSetShader(m_pD3D11VertexShader, nullptr, 0);
		immediateContext->PSSetShader(m_pD3D11PixelShader, nullptr, 0);
		immediateContext->VSSetConstantBuffers(0, 1, &m_pD3D11ConstantBuffer);


	}

	ImagePass::~ImagePass()
	{
		SAFE_RELEASE(m_pD3D11InputLayout);
		SAFE_RELEASE(m_pD3D11VertexBuffer);
		SAFE_RELEASE(m_pD3D11VertexShader);
		SAFE_RELEASE(m_pD3D11PixelShader);
	}

	void ImagePass::draw()
	{
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();

		ConstantBuffer cb;
		static float phi = 0.0f, theta = 0.0f;
		phi += 0.0001f, theta += 0.00015f;
		cb.world = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));
		//cb.world = XMMatrixIdentity();	// 单位矩阵的转置是它本身
		cb.view = XMMatrixTranspose(XMMatrixLookAtLH(
			XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
		cb.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 1.0f, 1000.0f));

		D3D11_MAPPED_SUBRESOURCE mappedData;
		immediateContext->Map(m_pD3D11ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy_s(mappedData.pData, sizeof(cb), &cb, sizeof(cb));
		immediateContext->Unmap(m_pD3D11ConstantBuffer, 0);

		immediateContext->DrawIndexed(36, 0, 0);
	}
}