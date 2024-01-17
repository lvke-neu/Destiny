#include "ParticleComponent.h"
#include "Engine/Blob.h"
#include "Graphics/Visual3D.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Material.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Node3D.h"
#include "Transform3D.h"

namespace Destiny
{
	using namespace DirectX;

	ParticleComponent::ParticleComponent()
	{
		std::shared_ptr<Blob> data = nullptr;
		struct ParticleVertex
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

		int count = 100;
		std::vector<ParticleVertex> vertices;
		vertices.resize(count);
		for (int i = 0; i < count; i++)
		{
			vertices[i].position = XMFLOAT3(0.0f, i / 100.0f, 0.0f);
			vertices[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		data.reset(new Blob(vertices.size() * sizeof(ParticleVertex)));
		memcpy_s(data->getData(), data->getLength(), vertices.data(), data->getLength());
		auto vertexBuffer = Engine::GetInstance()->getGraphicsSystem()->createVertexBuffer(sizeof(ParticleVertex), 0, data);
		vertexBuffer->load(0);
		m_visual3D->setVertexBuffer(vertexBuffer);

		std::vector<unsigned int> indices{ 0,1,2 };
		//indices.resize(count);
		//for (int i = 0; i < count; i++)
		//{
		//	indices[i] = i;
		//}
		data.reset(new Blob(sizeof(unsigned int) * indices.size()));
		memcpy_s(data->getData(), data->getLength(), indices.data(), data->getLength());
		auto indexBuffer = Engine::GetInstance()->getGraphicsSystem()->createIndexBuffer(DXGI_FORMAT_R32_UINT, data);
		indexBuffer->load(0);
		m_visual3D->setIndexBuffer(indexBuffer);

		D3D11_INPUT_ELEMENT_DESC inputElements[2] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		data.reset(new Blob(2 * sizeof(D3D11_INPUT_ELEMENT_DESC)));
		memcpy_s(data->getData(), data->getLength(), inputElements, data->getLength());
		auto inputLayout = Engine::GetInstance()->getGraphicsSystem()->createInputLayout(data, "assets://HLSL/Particle_VS.cso");
		inputLayout->load(0);
		m_visual3D->setInputLayout(inputLayout);

		auto vertexShader = Engine::GetInstance()->getGraphicsSystem()->createVertexShader("assets://HLSL/Particle_VS.cso");
		vertexShader->load(0);
		auto pixelShader = Engine::GetInstance()->getGraphicsSystem()->createPixelShader("assets://HLSL/Particle_PS.cso");
		pixelShader->load(0);
		m_visual3D->setVertexShader(vertexShader);
		m_visual3D->setPixelShader(pixelShader);
		
		m_visual3D->setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		m_visual3D->setDrawType(Visual3D::DrawType::DrawVertex);

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::Update, std::bind(&ParticleComponent::update, this, std::placeholders::_1));
	}

	void ParticleComponent::update(void* data)
	{
		if (!m_node)
		{
			return;
		}
		float deltaTime = *(float*)data;
		static float gravity = 9.8f;
		auto transform3D = m_node->get_transform3D();
		auto translation = transform3D.get_translation();
		static float sumDeltaTime = 0.0f;
		sumDeltaTime += deltaTime;
		float speed = gravity * sumDeltaTime;
		translation.y = translation.y - speed * sumDeltaTime;
		if (translation.y < 0.0f)
		{
			translation.y = 50.0f;
			sumDeltaTime = 0.0f;
		}
		transform3D.set_translation(translation);
		m_node->set_transform3D(transform3D);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ParticleComponent>("ParticleComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}