#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class RenderTargetView;
	class RenderToTextureComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		RenderToTextureComponent();
	public:
		void beforeDrawCommand();
	public:
		GET_CLASS_NAME(RenderToTextureComponent);
	private:
		std::shared_ptr<RenderTargetView> m_renderTargetView;
	};
}