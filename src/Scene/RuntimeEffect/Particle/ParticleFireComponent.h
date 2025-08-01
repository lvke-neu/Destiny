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
		virtual ~ParticleFireComponent() = default;
	public:
		virtual void onUpdate(float deltaTime) override;
		virtual void onPropertyChanged(const std::string& property) override;
	private:
		void update();
	private:
		unsigned int m_maxParticlesCount;
		std::shared_ptr<ComputerCommand> m_particleUpdateCS;
		std::shared_ptr<Texture> m_particlesBuffer;
	};
}