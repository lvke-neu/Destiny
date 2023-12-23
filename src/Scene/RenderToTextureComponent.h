#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class RenderTargetView;
	class DepthStencilView;
	class RenderToTextureComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		RenderToTextureComponent();
		virtual ~RenderToTextureComponent();
	public:
		void beforeDrawCommand();
		void onResize(void* data);
	public:
		GET_CLASS_NAME(RenderToTextureComponent);
	private:
		std::shared_ptr<RenderTargetView> m_renderTargetView;
		std::shared_ptr<DepthStencilView> m_depthStencilView;
	};
}