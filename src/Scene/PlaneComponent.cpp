#include "PlaneComponent.h"
#include "Engine/Blob.h"
#include "Graphics/Visual3D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Material.h"

namespace Destiny
{
	using namespace DirectX;
	PlaneComponent::PlaneComponent():
		m_width(100),
		m_height(100)
	{
		std::shared_ptr<Blob> data = nullptr;

		struct VertexPosTexcoord
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};

		std::vector<VertexPosTexcoord> vertices;
		vertices.resize(m_height * m_width);

		for (int i = 1; i <= m_height; i++)
		{
			for (int j = 1; j <= m_width; j++)
			{
				vertices[(i - 1) * m_width + j - 1].position = XMFLOAT3((float)(j - 1) * 5, 0, (float)-(i - 1) * 5);
				vertices[(i - 1) * m_width + j - 1].normal = XMFLOAT3(0, 1, 0);
				vertices[(i - 1) * m_width + j - 1].texcoord = XMFLOAT2(0, 0);
			}
		}

		data.reset(new Blob(m_width * m_height * sizeof(VertexPosTexcoord)));
		memcpy_s(data->getData(), data->getLength(), vertices.data(), data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosTexcoord), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		std::vector<unsigned int> indices;
		for (int i = 0; i < m_height; i++)
		{
			for (int j = 0; j < m_width - 1; j++)
			{
				indices.push_back(i * m_width + j);
				indices.push_back(i * m_width + j + 1);
			}
		}
		for (int i = 0; i < m_width; i++)
		{
			for (int j = 0; j < m_height - 1; j++)
			{
				indices.push_back(j * m_width + i);
				indices.push_back((j + 1) * m_width + i);
			}
		}

		data.reset(new Blob(sizeof(unsigned int) * indices.size()));
		memcpy_s(data->getData(), data->getLength(), indices.data(), data->getLength());
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
		auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/Basic_VS.cso");
		inputLayout->load(0);
		m_visual3D->setInputLayout(inputLayout);
		m_visual3D->setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/Basic_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/Basic_PS.cso");
		pixelShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);

		m_material->set_useColor(true);
		m_material->set_ambientColor(Color::White);
		m_material->set_diffuseColor(Color::Black);
		m_material->set_specularColor(Color::Black);
		m_material->load();
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<PlaneComponent>("PlaneComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}