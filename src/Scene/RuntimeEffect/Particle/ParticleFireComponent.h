#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class ParticleFireComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		ParticleFireComponent();
		virtual ~ParticleFireComponent(); // Changed from default to handle cleanup if needed

	public:
		virtual void onUpdate(float deltaTime) override;
		virtual void onEnterScene() override; // Add this to init position
		virtual void onPropertyChanged(const std::string& property) override;

	private:
		void initResources();

	private:
		unsigned int m_maxParticlesCount;
		float m_totalTime;
		std::shared_ptr<ComputerCommand> m_particleUpdateCS;
		std::shared_ptr<Texture> m_particlesBuffer;
	};
}
