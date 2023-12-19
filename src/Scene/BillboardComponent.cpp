#include "BillboardComponent.h"
#include "Engine/Blob.h"
#include "Graphics/Visual3D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/GeometryShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Material.h"

namespace Destiny
{
	using namespace DirectX;

	BillboardComponent::BillboardComponent()
	{
		std::shared_ptr<Blob> data = nullptr;
		struct VertexPosTexcoord
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};
		VertexPosTexcoord vertices[3];
		vertices[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertices[1].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertices[2].position = XMFLOAT3(0.0f, 0.0f, 0.0f);

		data.reset(new Blob(3 * sizeof(VertexPosTexcoord)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosTexcoord), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		data.reset(new Blob(sizeof(unsigned int) * 3));
		memcpy_s(data->getData(), data->getLength(), indices, data->getLength());
		auto indexBuffer = Engine::GetInstance()->getGraphicsSystem()->createIndexBuffer(DXGI_FORMAT_R32_UINT, data);
		indexBuffer->load(0);
		m_visual3D->setIndexBuffer(indexBuffer);


		D3D11_INPUT_ELEMENT_DESC inputElements[3] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		data.reset(new Blob(3 * sizeof(D3D11_INPUT_ELEMENT_DESC)));
		memcpy_s(data->getData(), data->getLength(), inputElements, data->getLength());
		auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/TestGS_VS.cso");
		inputLayout->load(0);
		m_visual3D->setInputLayout(inputLayout);

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/TestGS_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/TestGS_PS.cso");
		pixelShader->load(0);
		auto geometryShader = Engine::GetInstance()->getGraphicsSystem()->createGeometryShader("assets://HLSL/TestGS_GS.cso");
		geometryShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);
		m_visual3D->setGeometryShader(geometryShader);

		data.reset(new Blob(sizeof(D3D11_BLEND_DESC)));
		m_blendStateDesc.RenderTarget[0].BlendEnable = true;
		m_blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		m_blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		memcpy_s(data->getData(), data->getLength(), &m_blendStateDesc, data->getLength());
		auto blendState = Engine::GetInstance()->getGraphicsSystem()->createBlendState(data);
		blendState->load(0);
		m_visual3D->setBlendState(blendState);

		m_material->set_useColor(false);
		m_material->set_ambientTexturePath("assets://Texture/tree0.dds");
		m_material->load();
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BillboardComponent>("BillboardComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}