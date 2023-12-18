#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class SkyboxComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		SkyboxComponent();
	public:
		void beforeDrawCommand();
	public:
		GET_CLASS_NAME(SkyboxComponent);
		GET(std::string, path);

		void set_path(std::string path);
	private:
		std::shared_ptr<Texture> m_cubeTexture;
		std::shared_ptr<SamplerState> m_cubeSamplerState;
		std::string m_path;
	};
}