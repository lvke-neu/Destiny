#include "SceneManager.h"
#include "Graphics/Visual3D.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"


//TODO:REMOVE
#include "Engine/Blob.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/RasterizerState.h"
#include "Graphics/DepthStencilState.h"
#include "Graphics/BlendState.h"
#include <DirectXMath.h>

namespace Destiny
{
	SceneManager::SceneManager() :
		m_visual3D(nullptr)
	{

	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::initialize()
	{
		m_visual3D = std::make_shared<Visual3D>();

		std::shared_ptr<Blob> data = nullptr;

		DirectX::XMFLOAT3 vertices[3] =
		{
			{ -0.5f, 0.0f, 0.5f },
			{ 0.5f,  0.0f, 0.5f },
			{ 0.0f,  0.5f, 0.5f }
		};
		data.reset(new Blob(3 * sizeof(DirectX::XMFLOAT3)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(DirectX::XMFLOAT3), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		unsigned int indices[3] =
		{
			0, 2, 1
		};
		data.reset(new Blob(sizeof(unsigned int) * 3));
		memcpy_s(data->getData(), data->getLength(), indices, data->getLength());
		auto indexBuffer = Engine::GetInstance()->getGraphicsSystem()->createIndexBuffer(DXGI_FORMAT_R32_UINT, data);
		indexBuffer->load(0);
		m_visual3D->setIndexBuffer(indexBuffer);

		D3D11_INPUT_ELEMENT_DESC inputElements[1] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		data.reset(new Blob(sizeof(D3D11_INPUT_ELEMENT_DESC)));
		memcpy_s(data->getData(), data->getLength(), inputElements, data->getLength());
		auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/Triangle_VS.cso");
		inputLayout->load(0);
		m_visual3D->setInputLayout(inputLayout);

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/Triangle_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/Triangle_PS.cso");
		pixelShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);

		D3D11_RASTERIZER_DESC rasterizerDesc = RasterizerState::Default_Rasterizer_Desc;
		data.reset(new Blob(sizeof(D3D11_RASTERIZER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &rasterizerDesc, data->getLength());
		auto rasterizerState = Engine::GetInstance()->getGraphicsSystem()->createRasterizerState(data);
		rasterizerState->load(0);
		m_visual3D->setRasterizerState(rasterizerState);

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = DepthStencilState::Default_DepthStencil_Desc;
		data.reset(new Blob(sizeof(D3D11_DEPTH_STENCIL_DESC)));
		memcpy_s(data->getData(), data->getLength(), &depthStencilStateDesc, data->getLength());
		auto depthStencilState = Engine::GetInstance()->getGraphicsSystem()->createDepthStencilState(data);
		depthStencilState->load(0);
		m_visual3D->setDepthStencilState(depthStencilState);

		D3D11_BLEND_DESC blendStateDesc = BlendState::Default_BlendState_Desc;
		data.reset(new Blob(sizeof(D3D11_BLEND_DESC)));
		memcpy_s(data->getData(), data->getLength(), &blendStateDesc, data->getLength());
		auto blendState = Engine::GetInstance()->getGraphicsSystem()->createBlendState(data);
		blendState->load(0);
		m_visual3D->setBlendState(blendState);
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::update()
	{
		Engine::GetInstance()->getGraphicsSystem()->commitVisual3D(m_visual3D);
	}
}