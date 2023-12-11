#include "TestGSComponent.h"
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

	TestGSComponent::TestGSComponent()
	{
		std::shared_ptr<Blob> data = nullptr;
		struct VertexPosColor
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};
		VertexPosColor vertices[3];
		vertices[0].position = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[2].position = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		data.reset(new Blob(3 * sizeof(VertexPosColor)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosColor), 0, data);
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

		m_material->set_useColor(true);
		m_material->set_ambientColor({ 1.0f, 1.0f, 0.0f, 1.0f });
		m_material->load();
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TestGSComponent>("TestGSComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}