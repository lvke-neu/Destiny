#pragma once
#include "Visual3DComponent.h"
#include "Graphics/ConstantBuffer.h"
#include <DirectXMath.h>

namespace Destiny
{
	using namespace DirectX;
	class SamplerState;
	class Texture;
	class SphereComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		SphereComponent();
	public:
		virtual void onAttachNode() override;
		virtual void onNodeTransformChanged() override;
	public:
		GET_CLASS_NAME(SphereComponent);
		GET(std::string, texturePath);

		void set_texturePath(std::string texturePath);
	private:
		void beforeDrawCommands();
	private:
		std::shared_ptr<ConstantBuffer<XMMATRIX>> m_worldMatrix;

		std::shared_ptr<SamplerState> m_samplerState;
		std::string m_texturePath;
		std::shared_ptr<Texture> m_texture;
	};
}