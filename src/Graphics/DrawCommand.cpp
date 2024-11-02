#include "DrawCommand.h"
#include "DrawParameters.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "SamplerState.h"
#include <d3d11.h>

namespace Destiny
{
	void DrawCommand::execute(ID3D11DeviceContext* deviceContext)
	{
		for (const auto& drawParameters : m_drawParameters)
		{
			if (!drawParameters)
			{
				continue;
			}

			deviceContext->IASetVertexBuffers(0, 1, &drawParameters->vertexBuffer, &drawParameters->stride, &drawParameters->offset);
			deviceContext->IASetIndexBuffer(drawParameters->indexBuffer, (DXGI_FORMAT)drawParameters->format, 0);
			deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)drawParameters->primitiveTopology);
			deviceContext->IASetInputLayout(drawParameters->inputLayout);

			deviceContext->VSSetShader(drawParameters->vertexShader, nullptr, 0);
			deviceContext->PSSetShader(drawParameters->pixelShader, nullptr, 0);

			deviceContext->RSSetState(drawParameters->rasterizerState);
			deviceContext->OMSetDepthStencilState(drawParameters->depthStencilState, 0);
			deviceContext->OMSetBlendState(drawParameters->blendState, nullptr, 0xFFFFFFFF);

			for (const auto& constantBuffer : drawParameters->constantBuffers)
			{
				if (constantBuffer.second)
				{
					constantBuffer.second->bind();
				}
			}

			for (const auto& texture : drawParameters->textures)
			{
				if (texture.second.second)
				{
					texture.second.second->bind(texture.second.first);
				}
			}

			for (const auto& samplerState : drawParameters->samplerStates)
			{
				if (samplerState.second.second)
				{
					samplerState.second.second->bind(samplerState.second.first);
				}
			}

			switch (drawParameters->drawType)
			{
			case 1:
				deviceContext->Draw(drawParameters->vertexCount, 0);
			case 2:
				deviceContext->DrawIndexed(drawParameters->indexCount, 0, 0);
			}
		}
		m_drawParameters.clear();
	}

	void DrawCommand::addDrawParameter(std::shared_ptr<DrawParameters> drawParameters)
	{
		m_drawParameters.emplace(drawParameters);
	}
}