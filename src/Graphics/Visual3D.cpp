#include "Visual3D.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/RasterizerState.h"
#include "Graphics/DepthStencilState.h"
#include "Graphics/BlendState.h"
#include "Graphics/SamplerState.h"
#include "Graphics/ConstantBuffer.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace Destiny
{
	static std::shared_ptr<ConstantBuffer<DirectX::XMFLOAT4>> constantBuffer = nullptr;

	Visual3D::Visual3D()
	{
		m_immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();

		auto graphicsSystem = Engine::GetInstance()->getGraphicsSystem();
		std::shared_ptr<Blob> data = nullptr;

		DirectX::XMFLOAT3 vertices[3] =
		{
			{ -0.5f, 0.0f, 0.5f },
			{ 0.5f,  0.0f, 0.5f },
			{ 0.0f,  0.5f, 0.5f }
		};
		data.reset(new Blob(3 * sizeof(DirectX::XMFLOAT3)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		m_vertexBuffer = graphicsSystem->createVertexBuffer(sizeof(DirectX::XMFLOAT3), 0, data);
		m_vertexBuffer->load(0);

		unsigned int indices[3] =
		{
			0, 2, 1
		};
		data.reset(new Blob(sizeof(unsigned int) * 3));
		memcpy_s(data->getData(), data->getLength(), indices, data->getLength());
		m_indexBuffer = graphicsSystem->createIndexBuffer(DXGI_FORMAT_R32_UINT, data);
		m_indexBuffer->load(0);

		D3D11_INPUT_ELEMENT_DESC inputElements[1] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		data.reset(new Blob(sizeof(D3D11_INPUT_ELEMENT_DESC)));
		memcpy_s(data->getData(), data->getLength(), inputElements, data->getLength());
		m_inputLayout = graphicsSystem->createInputLayout(data, "assets://HLSL/Triangle_VS.cso");
		m_inputLayout->load(0);

		m_vertexShader = graphicsSystem->createVertexShader("assets://HLSL/Triangle_VS.cso");
		m_vertexShader->load(0);
		m_pixelShader = graphicsSystem->createPixelShader("assets://HLSL/Triangle_PS.cso");
		m_pixelShader->load(0);


		D3D11_RASTERIZER_DESC rasterizerDesc = RasterizerState::Default_Rasterizer_Desc;
		data.reset(new Blob(sizeof(D3D11_RASTERIZER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &rasterizerDesc, data->getLength());
		m_rasterizerState = Engine::GetInstance()->getGraphicsSystem()->createRasterizerState(data);
		m_rasterizerState->load(0);

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = DepthStencilState::Default_DepthStencil_Desc;
		data.reset(new Blob(sizeof(D3D11_DEPTH_STENCIL_DESC)));
		memcpy_s(data->getData(), data->getLength(), &depthStencilStateDesc, data->getLength());
		m_depthStencilState = Engine::GetInstance()->getGraphicsSystem()->createDepthStencilState(data);
		m_depthStencilState->load(0);

		D3D11_BLEND_DESC blendStateDesc = BlendState::Default_BlendState_Desc;
		data.reset(new Blob(sizeof(D3D11_BLEND_DESC)));
		memcpy_s(data->getData(), data->getLength(), &blendStateDesc, data->getLength());
		m_blendState = Engine::GetInstance()->getGraphicsSystem()->createBlendState(data);
		m_blendState->load(0);

		D3D11_SAMPLER_DESC samplerStateDesc = SamplerState::Default_SamplerState_Desc;
		data.reset(new Blob(sizeof(D3D11_SAMPLER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &samplerStateDesc, data->getLength());
		m_samplerState = Engine::GetInstance()->getGraphicsSystem()->createSamplerState(data);
		m_samplerState->load(0);

		constantBuffer = std::make_shared<ConstantBuffer<DirectX::XMFLOAT4>>();

		constantBuffer->update({ 0.0f, 0.0f, 1.0f, 1.0f });
	}

	Visual3D::~Visual3D()
	{

	}

	void Visual3D::draw()
	{
		if (!m_vertexBuffer->isLoadingSucceed()      ||
			!m_indexBuffer->isLoadingSucceed()       ||
			!m_inputLayout->isLoadingSucceed()       ||
			!m_vertexShader->isLoadingSucceed()      ||
			!m_pixelShader->isLoadingSucceed()	     ||
			!m_rasterizerState->isLoadingSucceed()   ||
			!m_depthStencilState->isLoadingSucceed() ||
			!m_blendState->isLoadingSucceed()		 
			)

		{
			return;
		}

		auto graphicsSystem = Engine::GetInstance()->getGraphicsSystem();

		//IA
		m_immediateContext->IASetVertexBuffers(0, 1, m_vertexBuffer->getVertexBuffer(), m_vertexBuffer->getStride(), m_vertexBuffer->getOffset());
		m_immediateContext->IASetIndexBuffer(m_indexBuffer->getIndexBuffer(), m_indexBuffer->getFormat(), 0);
		m_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_immediateContext->IASetInputLayout(m_inputLayout->getInputLayout());
		
		//SHDAER
		m_immediateContext->VSSetShader(m_vertexShader->getVertexShader(), nullptr, 0);
		m_immediateContext->PSSetShader(m_pixelShader->getPixelShader(), nullptr, 0);
		m_immediateContext->PSSetConstantBuffers(0, 1, constantBuffer->getConstantBuffer());

		//RS
		m_immediateContext->RSSetViewports(1, graphicsSystem->getViewport());
		m_immediateContext->RSSetState(m_rasterizerState->getRasterizerState());
		//OM
		m_immediateContext->OMSetRenderTargets(1, graphicsSystem->getRenderTargetView(), graphicsSystem->getDepthStencilView());
		m_immediateContext->OMSetDepthStencilState(m_depthStencilState->getDepthStencilState(), 0);
		m_immediateContext->OMSetBlendState(m_blendState->getBlendState(), nullptr, 0xFFFFFFFF);

		m_immediateContext->DrawIndexed(m_indexBuffer->getCount(), 0, 0);
	}
}