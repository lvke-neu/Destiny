#pragma once
#include "Graphics/VisualComponent.h"
#include <DirectXMath.h> // For XMFLOAT3

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class ParticleWaterComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		ParticleWaterComponent();
		virtual ~ParticleWaterComponent();

	public:
		GET(unsigned int, maxParticlesCount);
		void set_maxParticlesCount(unsigned int maxParticlesCount);

		// Exposed Parameters
		GET(DirectX::XMFLOAT3, gravity);
		void set_gravity(DirectX::XMFLOAT3 gravity);

		GET(float, spreadRadius);
		void set_spreadRadius(float spreadRadius);

		GET(float, splashHeight);
		void set_splashHeight(float splashHeight);

		GET(float, particleMinSize);
		void set_particleMinSize(float particleMinSize);

		GET(float, particleMaxSize);
		void set_particleMaxSize(float particleMaxSize);

		GET(float, particleMinLife);
		void set_particleMinLife(float particleMinLife);

		GET(float, particleMaxLife);
		void set_particleMaxLife(float particleMaxLife);

	public:
		virtual void onUpdate(float deltaTime) override;
		virtual void onEnterScene() override;
		virtual void onPropertyChanged(const std::string& property) override;

	private:
		void initResources();
		void updateConstants(); // Helper to update shader constants

	private:
		unsigned int m_maxParticlesCount;
		float m_totalTime;
		
		// Parameter Storage
		DirectX::XMFLOAT3 m_gravity;
		float m_spreadRadius;
		float m_splashHeight;
		float m_particleMinSize;
		float m_particleMaxSize;
		float m_particleMinLife;
		float m_particleMaxLife;

		std::shared_ptr<ComputerCommand> m_particleUpdateCS;
		std::shared_ptr<Texture> m_particlesBuffer;
	};
}
