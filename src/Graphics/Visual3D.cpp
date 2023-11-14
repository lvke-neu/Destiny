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
#include <d3d11.h>
#include <DirectXMath.h>

namespace Destiny
{
	Visual3D::Visual3D() :
		m_commandList(nullptr)
	{
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


		///**********************************************command*************************************/
		//auto deferredContext = graphicsSystem->getDeferredContext();

		//deferredContext->IASetVertexBuffers(0, 1, m_vertexBuffer->getVertexBuffer(), m_vertexBuffer->getStride(), m_vertexBuffer->getOffset());
		//deferredContext->IASetIndexBuffer(m_indexBuffer->getIndexBuffer(), m_indexBuffer->getFormat(), 0);
		//deferredContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//deferredContext->IASetInputLayout(m_inputLayout->getInputLayout());

		//deferredContext->VSSetShader(m_vertexShader->getVertexShader(), nullptr, 0);
		//deferredContext->PSSetShader(m_pixelShader->getPixelShader(), nullptr, 0);

		//deferredContext->DrawIndexed(m_indexBuffer->getCount(), 0, 0);

		//deferredContext->FinishCommandList(false, &m_commandList);
		///******************************************************************************************/
	}

	Visual3D::~Visual3D()
	{
		SAFE_RELEASE(m_commandList);
	}

	void Visual3D::draw()
	{
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();

		immediateContext->IASetVertexBuffers(0, 1, m_vertexBuffer->getVertexBuffer(), m_vertexBuffer->getStride(), m_vertexBuffer->getOffset());
		immediateContext->IASetIndexBuffer(m_indexBuffer->getIndexBuffer(), m_indexBuffer->getFormat(), 0);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		immediateContext->IASetInputLayout(m_inputLayout->getInputLayout());

		immediateContext->VSSetShader(m_vertexShader->getVertexShader(), nullptr, 0);
		immediateContext->PSSetShader(m_pixelShader->getPixelShader(), nullptr, 0);

		immediateContext->DrawIndexed(m_indexBuffer->getCount(), 0, 0);
	}

	ID3D11CommandList* Visual3D::getCommmandList()
	{
		return m_commandList;
	}
}