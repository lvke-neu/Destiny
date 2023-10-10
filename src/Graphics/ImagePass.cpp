#include "ImagePass.h"
#include "Engine/Engine.h"
#include "Engine/Utility.h"
#include "GraphicsSystem.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace Destiny
{
	ImagePass::ImagePass()
	{
		ID3DBlob* blob{ nullptr };

		auto device = Engine::GetInstance()->getGraphicsSystem()->getDevice();

		if (D3DReadFileToBlob(L"HLSL/Triangle_VS.cso", &blob) == S_OK)
		{
			device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pD3D11VertexShader);

			D3D11_INPUT_ELEMENT_DESC inputLayout[2] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			device->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout), blob->GetBufferPointer(), blob->GetBufferSize(), &m_pD3D11InputLayout);
		}

		SAFE_RELEASE(blob);
		if (D3DReadFileToBlob(L"HLSL/Triangle_PS.cso", &blob) == S_OK)
		{
			device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pD3D11PixelShader);
		}

		struct VertexPosColor
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
		};
		// 设置三角形顶点
		VertexPosColor vertices[] =
		{
			{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.5f),   DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),  DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
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


		// ******************
		// 给渲染管线各个阶段绑定好所需资源
		//

		// 输入装配阶段的顶点缓冲区设置
		UINT stride = sizeof(VertexPosColor);	// 跨越字节数
		UINT offset = 0;						// 起始偏移量

		auto deferredDeviceContext = Engine::GetInstance()->getGraphicsSystem()->getDeferredContext();
		
		static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		auto rtv = Engine::GetInstance()->getGraphicsSystem()->getRenderTargetView();
		deferredDeviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		deferredDeviceContext->ClearRenderTargetView(rtv, color);
		
		deferredDeviceContext->IASetVertexBuffers(0, 1, &m_pD3D11VertexBuffer, &stride, &offset);
		// 设置图元类型，设定输入布局
		deferredDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deferredDeviceContext->IASetInputLayout(m_pD3D11InputLayout);
		// 将着色器绑定到渲染管线
		deferredDeviceContext->VSSetShader(m_pD3D11VertexShader, nullptr, 0);
		deferredDeviceContext->PSSetShader(m_pD3D11PixelShader, nullptr, 0);
		deferredDeviceContext->Draw(3, 0);
		deferredDeviceContext->FinishCommandList(false, &m_pD3D11CommandList);

	}

	ImagePass::~ImagePass()
	{
		SAFE_RELEASE(m_pD3D11CommandList);
		SAFE_RELEASE(m_pD3D11InputLayout);
		SAFE_RELEASE(m_pD3D11VertexBuffer);
		SAFE_RELEASE(m_pD3D11VertexShader);
		SAFE_RELEASE(m_pD3D11PixelShader);
	}

	void ImagePass::draw()
	{
		struct VertexPosColor
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
		};
		UINT stride = sizeof(VertexPosColor);	// 跨越字节数
		UINT offset = 0;						// 起始偏移量
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		immediateContext->IASetVertexBuffers(0, 1, &m_pD3D11VertexBuffer, &stride, &offset);
		// 设置图元类型，设定输入布局
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		immediateContext->IASetInputLayout(m_pD3D11InputLayout);
		// 将着色器绑定到渲染管线
		immediateContext->VSSetShader(m_pD3D11VertexShader, nullptr, 0);
		immediateContext->PSSetShader(m_pD3D11PixelShader, nullptr, 0);
		immediateContext->Draw(3, 0);
	}
}