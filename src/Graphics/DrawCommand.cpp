#include "DrawCommand.h"
#include "DrawParameters.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "SamplerState.h"
#include "GraphicsPipeline/GraphicsCommandList.h"
#include <d3d11.h>

namespace Destiny
{
	void DrawCommand::execute(std::shared_ptr<GraphicsContext> deviceContext)
	{
#if 0
		for (const auto& drawParameters : m_drawParameters)
		{
			if (!drawParameters)
			{
				continue;
			}

			switch (drawParameters->drawType)
			{
			case 1:
			case 2:
			case 4:
			{
				deviceContext->IASetVertexBuffers(0, drawParameters->vertexBuffer ? 1 : 0 , &drawParameters->vertexBuffer, &drawParameters->vertexBuffer_stride, &drawParameters->vertexBuffer_offset);
				break;
			}
			case 3:
			{
				UINT strides[2] = { drawParameters->vertexBuffer_stride, drawParameters->instanceBuffer_stride };
				UINT offsets[2] = { drawParameters->vertexBuffer_offset, drawParameters->instanceBuffer_offset };
				ID3D11Buffer* buffers[2] = { drawParameters->vertexBuffer, drawParameters->instanceBuffer };
				deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
				break;
			}
			}
			
			deviceContext->IASetIndexBuffer(drawParameters->indexBuffer, (DXGI_FORMAT)drawParameters->format, 0);
			deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)drawParameters->primitiveTopology);
			deviceContext->IASetInputLayout(drawParameters->inputLayout);

			deviceContext->VSSetShader(drawParameters->vertexShader, nullptr, 0);
			deviceContext->PSSetShader(drawParameters->pixelShader, nullptr, 0);
			deviceContext->GSSetShader(drawParameters->geometryShader, nullptr, 0);
			deviceContext->HSSetShader(drawParameters->hullShader, nullptr, 0);
			deviceContext->DSSetShader(drawParameters->domainShader, nullptr, 0);

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
				break;
			case 2:
				deviceContext->DrawIndexed(drawParameters->indexCount, 0, 0);
				break;
			case 3:
				deviceContext->DrawIndexedInstanced(drawParameters->indexCount, drawParameters->instanceCount, 0, 0, 0);
				break;
			case 4:
			{
				for (const auto& drawIndirectMethod_indirectBufferOffset : drawParameters->drawIndirectMethod_indirectBufferOffsets)
				{
					switch (drawIndirectMethod_indirectBufferOffset.first)
					{
					case 1:
						deviceContext->DrawInstancedIndirect(drawParameters->indirectBuffer, drawIndirectMethod_indirectBufferOffset.second);
						break;
					case 2:
						deviceContext->DrawIndexedInstancedIndirect(drawParameters->indirectBuffer, drawIndirectMethod_indirectBufferOffset.second);
						break;
					}
				}

			}
			}

			//restore all
			ID3D11Buffer* nullBuffer = nullptr;
			unsigned int stride = 0;
			unsigned int offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED);
			deviceContext->IASetInputLayout(nullptr);

			deviceContext->VSSetShader(nullptr, nullptr, 0);
			deviceContext->PSSetShader(nullptr, nullptr, 0);
			deviceContext->GSSetShader(nullptr, nullptr, 0);

			deviceContext->RSSetState(nullptr);
			deviceContext->OMSetDepthStencilState(nullptr, 0);
			deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
			
			for (const auto& constantBuffer : drawParameters->constantBuffers)
			{
				if (constantBuffer.second)
				{
					constantBuffer.second->unBind();
				}
			}

			for (const auto& texture : drawParameters->textures)
			{
				if (texture.second.second)
				{
					texture.second.second->unBind(texture.second.first);
				}
			}

			for (const auto& samplerState : drawParameters->samplerStates)
			{
				if (samplerState.second.second)
				{
					samplerState.second.second->unBind(samplerState.second.first);
				}
			}
		}
#endif
	}

	void DrawCommand::addDrawParameter(std::shared_ptr<DrawParameters> drawParameters)
	{
		m_drawParameters.emplace(drawParameters);
	}

	void DrawCommand::clearDrawParameter()
	{
		m_drawParameters.clear();
	}
}