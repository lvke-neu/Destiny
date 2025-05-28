#pragma once
#include "Graphics/VisualComponent.h"
#include "Math/Color.h"

namespace Destiny
{
	class SamplerState;
	class SkyboxComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		SkyboxComponent();
		virtual ~SkyboxComponent() = default;
	public:
		virtual void				onNodeTransformChanged() override;
	public:
		GET(std::string, texture);
		void set_texture(std::string texture);

		GET(float, exposure);
		void set_exposure(float exposure);
	private:
		void					notifyVisualRendererConstantChanged(std::shared_ptr<Node> node);
	private:
		std::string m_texture;
		float m_exposure;
		std::shared_ptr<SamplerState> m_cubeSampler;
	};
}