#include "PBRSphereComponent.h"
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

	PBRSphereComponent::PBRSphereComponent()
	{
		std::shared_ptr<Blob> data = nullptr;
		struct VertexPosTexcoord
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 texcoord;
		};

		const float radius = 1.0f;
		const UINT levels = 20;
		const UINT slices = 20;

		const UINT vertexCount = 2 + (levels - 1) * (slices + 1);
		VertexPosTexcoord vertices[vertexCount];

		DWORD vIndex = 0, iIndex = 0;

		float phi = 0.0f, theta = 0.0f;
		float per_phi = XM_PI / levels;
		float per_theta = XM_2PI / slices;
		float x, y, z;

		vertices[vIndex++] = VertexPosTexcoord({ XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });

		for (UINT i = 1; i < levels; ++i)
		{
			phi = per_phi * i;

			for (UINT j = 0; j <= slices; ++j)
			{
				theta = per_theta * j;
				x = radius * sinf(phi) * cosf(theta);
				y = radius * cosf(phi);
				z = radius * sinf(phi) * sinf(theta);

				XMFLOAT3 pos = XMFLOAT3(x, y, z), normal;
				XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

				vertices[vIndex++] = VertexPosTexcoord({ pos, normal, XMFLOAT2(theta / XM_2PI, phi / XM_PI) });
			}
		}
		vertices[vIndex++] = VertexPosTexcoord({ XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) });
		
		data.reset(new Blob(vertexCount * sizeof(VertexPosTexcoord)));
		memcpy_s(data->getData(), data->getLength(), vertices, data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(VertexPosTexcoord), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		unsigned int indices[6 * (levels - 1) * slices];
		if (levels > 1)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = 0;
				indices[iIndex++] = j % (slices + 1) + 1;
				indices[iIndex++] = j;
			}
		}
		for (UINT i = 1; i < levels - 1; ++i)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = (i - 1) * (slices + 1) + j;
				indices[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

				indices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = i * (slices + 1) + j;
				indices[iIndex++] = (i - 1) * (slices + 1) + j;
			}
		}
		if (levels > 1)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				indices[iIndex++] = (levels - 2) * (slices + 1) + j;
				indices[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
				indices[iIndex++] = (levels - 1) * (slices + 1) + 1;
			}
		}

		data.reset(new Blob(sizeof(unsigned int) * 6 * (levels - 1) * slices));
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
		auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/PBR_VS.cso");
		inputLayout->load(0);
		m_visual3D->setInputLayout(inputLayout);

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/PBR_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/PBR_PS.cso");
		pixelShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);

		m_material->set_useColor(true);
		m_material->set_ambientColor({0.2f, 0.2f, 0.2f, 1.0f});
		m_material->set_diffuseColor({ 0.8f, 0.8f, 0.8f, 1.0f });
		m_material->set_specularColor({ 0.5f, 0.5f, 0.5f, 1.0f });
		m_material->load();

		m_albedo = Color::Red;
		m_metallic = 0.5f;
		m_roughness = 0.5f;
		m_ao = 1.0f;


		m_pbrMaterial = std::make_shared<ConstantBuffer<cbPBRMaterial>>();
		m_visual3D->registerBeforeDrawCommands(std::bind(&PBRSphereComponent::updateConstantbuffer, this));
	}

	void PBRSphereComponent::set_albedo(Color albedo)
	{
		m_albedo = albedo;
	}

	void PBRSphereComponent::set_metallic(float metallic)
	{
		m_metallic = metallic;
	}

	void PBRSphereComponent::set_roughness(float roughness)
	{
		m_roughness = roughness;
	}

	void PBRSphereComponent::set_ao(float ao)
	{
		m_ao = ao;
	}

	void PBRSphereComponent::updateConstantbuffer()
	{
		cbPBRMaterial pbrMaterial;
		pbrMaterial.albedo = { m_albedo.get_r(), m_albedo.get_g(), m_albedo.get_b(), m_albedo.get_a() };
		pbrMaterial.metallic = m_metallic;
		pbrMaterial.roughness = m_roughness;
		pbrMaterial.ao = m_ao;

		m_pbrMaterial->update(pbrMaterial);

		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetConstantBuffers(6, 1, m_pbrMaterial->getConstantBuffer());
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<PBRSphereComponent>("PBRSphereComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
			.property("albedo", &PBRSphereComponent::get_albedo, &PBRSphereComponent::set_albedo)
			.property("metallic", &PBRSphereComponent::get_metallic, &PBRSphereComponent::set_metallic)
			.property("roughness", &PBRSphereComponent::get_roughness, &PBRSphereComponent::set_roughness)
			.property("ao", &PBRSphereComponent::get_ao, &PBRSphereComponent::set_ao);
	}
}