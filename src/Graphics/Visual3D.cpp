#include "Visual3D.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/RasterizerState.h"
#include "Graphics/DepthStencilState.h"
#include "Graphics/BlendState.h"
#include <d3d11.h>

namespace Destiny
{
	Visual3D::Visual3D() :
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_inputLayout(nullptr),
		m_vertexShader(nullptr),
		m_pixelShader(nullptr),
		m_rasterizerState(nullptr),
		m_depthStencilState(nullptr),
		m_blendState(nullptr)
	{

	}

	Visual3D::~Visual3D()
	{

	}

	void Visual3D::draw()
	{
		if (
			!m_vertexBuffer || !m_vertexBuffer->isLoadingSucceed() ||
			!m_indexBuffer || !m_indexBuffer->isLoadingSucceed() ||
			!m_vertexShader || !m_vertexShader->isLoadingSucceed() ||
			!m_pixelShader || !m_pixelShader->isLoadingSucceed() ||
			!m_rasterizerState || !m_rasterizerState->isLoadingSucceed() ||
			!m_depthStencilState || !m_depthStencilState->isLoadingSucceed() ||
			!m_blendState || !m_blendState->isLoadingSucceed()
			)
		{
			return;
		}

		auto graphicsSystem = Engine::GetInstance()->getGraphicsSystem();
		auto immediateContext = graphicsSystem->getImmediateContext();
		//IA
		immediateContext->IASetVertexBuffers(0, 1, m_vertexBuffer->getVertexBuffer(), m_vertexBuffer->getStride(), m_vertexBuffer->getOffset());
		immediateContext->IASetIndexBuffer(m_indexBuffer->getIndexBuffer(), m_indexBuffer->getFormat(), 0);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		immediateContext->IASetInputLayout(m_inputLayout->getInputLayout());
		
		//SHDAER
		immediateContext->VSSetShader(m_vertexShader->getVertexShader(), nullptr, 0);
		immediateContext->PSSetShader(m_pixelShader->getPixelShader(), nullptr, 0);

		//RS
		immediateContext->RSSetViewports(1, graphicsSystem->getViewport());
		immediateContext->RSSetState(m_rasterizerState->getRasterizerState());
		//OM
		immediateContext->OMSetRenderTargets(1, graphicsSystem->getRenderTargetView(), graphicsSystem->getDepthStencilView());
		immediateContext->OMSetDepthStencilState(m_depthStencilState->getDepthStencilState(), 0);
		immediateContext->OMSetBlendState(m_blendState->getBlendState(), nullptr, 0xFFFFFFFF);

		for (const auto& command : m_beforeDrawCommands)
		{
			if (command)
			{
				command();
			}
		}
		immediateContext->DrawIndexed(m_indexBuffer->getCount(), 0, 0);
		for (const auto& command : m_afterDrawCommands)
		{
			if (command)
			{
				command();
			}
		}
	}
}