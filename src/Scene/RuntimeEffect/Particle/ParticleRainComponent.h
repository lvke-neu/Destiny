#pragma once
#include "Graphics/VisualComponent.h"
#include <DirectXMath.h>

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class ParticleRainComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		ParticleRainComponent();
		virtual ~ParticleRainComponent();

	public:
		GET(unsigned int, maxParticlesCount);
		void set_maxParticlesCount(unsigned int maxParticlesCount);

		GET(DirectX::XMFLOAT3, rainAreaSize);
		void set_rainAreaSize(DirectX::XMFLOAT3 rainAreaSize);

		GET(float, fallSpeed);
		void set_fallSpeed(float fallSpeed);
		
		GET(float, dropLength);
		void set_dropLength(float dropLength);

	public:
		virtual void onUpdate(float deltaTime) override;
		virtual void onEnterScene() override;
		virtual void onPropertyChanged(const std::string& property) override;

	private:
		void initResources();
		void updateConstants();

	private:
		unsigned int m_maxParticlesCount;
		float m_totalTime;
		
		DirectX::XMFLOAT3 m_rainAreaSize;
		float m_fallSpeed;
		float m_dropLength;

		std::shared_ptr<ComputerCommand> m_particleUpdateCS;
		std::shared_ptr<Texture> m_particlesBuffer;
	};
}
