#include "Visual3D.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "InputLayout.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "SamplerState.h"

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
		m_blendState(nullptr),
		m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
		m_renderToMask(RenderToMask::render_to_scene),
		m_drawType(DrawType::DrawIndex)
	{

	}

	Visual3D::~Visual3D()
	{

	}

	void Visual3D::draw(RenderToMask mask)
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
		immediateContext->IASetPrimitiveTopology(m_primitiveTopology);
		immediateContext->IASetInputLayout(m_inputLayout->getInputLayout());
		
		//SHDAER
		immediateContext->VSSetShader(m_vertexShader->getVertexShader(), nullptr, 0);
		immediateContext->PSSetShader(m_pixelShader->getPixelShader(), nullptr, 0);
		if (m_geometryShader && m_geometryShader->isLoadingSucceed())
		{
			immediateContext->GSSetShader(m_geometryShader->getGeometryShader(), nullptr, 0);
		}
		else
		{
			immediateContext->GSSetShader(nullptr, nullptr, 0);
		}

		//RS
		immediateContext->RSSetViewports(1, graphicsSystem->getViewport());
		immediateContext->RSSetState(m_rasterizerState->getRasterizerState());
		//
		immediateContext->OMSetBlendState(m_blendState->getBlendState(), nullptr, 0xFFFFFFFF);
		immediateContext->OMSetDepthStencilState(m_depthStencilState->getDepthStencilState(), 0);

		if (mask == RenderToMask::render_to_shadow_map)
		{
			immediateContext->OMSetRenderTargets(1, graphicsSystem->getRenderToShadowMapRTV()->getRenderTargetView(), graphicsSystem->getRenderToShadowMapDSV()->getDepthStencilView());

			for (const auto& command : m_beforeDrawCommands)
			{
				if (command)
				{
					command();
				}
			}

			immediateContext->VSSetShader(graphicsSystem->getShadowMapVertexShader()->getVertexShader(), nullptr, 0);
			immediateContext->PSSetShader(nullptr, nullptr, 0);

			if (m_drawType == DrawType::DrawVertex)
			{
				immediateContext->Draw(m_vertexBuffer->getCount(), 0);
			}
			else if (m_drawType == DrawType::DrawIndex)
			{
				immediateContext->DrawIndexed(m_indexBuffer->getCount(), 0, 0);
			}
			for (const auto& command : m_afterDrawCommands)
			{
				if (command)
				{
					command();
				}
			}
		}
		else if (mask == RenderToMask::render_to_scene)
		{
			 immediateContext->OMSetRenderTargets(1, graphicsSystem->getRenderTargetView(), graphicsSystem->getDepthStencilView());

			 for (const auto& command : m_beforeDrawCommands)
			 {
				 if (command)
				 {
					 command();
				 }
			 }

			 immediateContext->PSSetSamplers(3, 1, graphicsSystem->getShadowMapSamplerState()->getSamplerState());
			 immediateContext->PSSetShaderResources(3, 1, graphicsSystem->getRenderToShadowMapDSV()->getShaderResourceView());

			 if (m_drawType == DrawType::DrawVertex)
			 {
				 immediateContext->Draw(m_vertexBuffer->getCount(), 0);
			 }
			 else if (m_drawType == DrawType::DrawIndex)
			 {
				 immediateContext->DrawIndexed(m_indexBuffer->getCount(), 0, 0);
			 }
			 for (const auto& command : m_afterDrawCommands)
			 {
				 if (command)
				 {
					 command();
				 }
			 }
		}
	}
}