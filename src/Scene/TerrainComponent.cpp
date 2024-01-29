#include "TerrainComponent.h"
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
	TerrainComponent::TerrainComponent() :
		m_width(200),
		m_height(200),
		m_m(200),
		m_n(200)
	{
		updateMesh();
		m_material->set_useColor(false);
		m_material->set_ambientTexturePath("assets://Texture/stone.dds");
		m_material->load();
	}

	void TerrainComponent::updateMesh()
	{
		std::shared_ptr<Blob> data = nullptr;

		struct VertexPosTexcoord
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};

		std::vector<VertexPosTexcoord> vertices;
		vertices.resize(m_m * m_n);

		float halfWidth = 0.5f * m_width;
		float halfHeight = 0.5f * m_height;
		float dx = ((float)m_width) / (m_n - 1);
		float dz = ((float)m_height) / (m_m - 1);
		float du = 1.0f / (m_n - 1);
		float dv = 1.0f / (m_m - 1);

		for (int i = 0; i < m_m; i++)
		{
			float z = halfHeight - i * dz;
			for (int j = 0; j < m_n; j++)
			{
				float x = -halfWidth + j * dx;

				float y = 0.1f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
				vertices[i * m_n + j].position = XMFLOAT3(x, y, z);
				vertices[i * m_n + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertices[i * m_n + j].texcoord = XMFLOAT2(j * du, i * dv);
			}
		}

		data.reset(new Blob(vertices.size() * sizeof(VertexPosTexcoord)));
		memcpy_s(data->getData(), data->getLength(), vertices.data(), data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosTexcoord), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		std::vector<unsigned int> indices;
		indices.resize(2 * (m_m - 1) * (m_n - 1) * 3);
		int k = 0;
		for (int i = 0; i < m_m - 1; ++i)
		{
			for (int j = 0; j < m_n - 1; ++j)
			{
				indices[k] = i * m_n + j;
				indices[k + 1] = i * m_n + j + 1;
				indices[k + 2] = (i + 1) * m_n + j;
				indices[k + 3] = (i + 1) * m_n + j;
				indices[k + 4] = i * m_n + j + 1;
				indices[k + 5] = (i + 1) * m_n + j + 1;
				k += 6;
			}
		}

		data.reset(new Blob(indices.size() * sizeof(unsigned int)));
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

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/Basic_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/Basic_PS.cso");
		pixelShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);

		//m_material->set_useColor(true);
		//m_material->set_ambientColor({ 145.0f / 255.0f, 178.0f / 255.0f, 164.0f / 255.0f, 1.0f });
		//m_material->set_diffuseColor(Color::Black);
		//m_material->set_specularColor({ 0.5f, 0.5f, 0.5f, 1.0f });
		//m_material->set_useColor(false);
		//m_material->set_ambientTexturePath("assets://Texture/stone.dds");

		//m_material->load();
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TerrainComponent>("TerrainComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}