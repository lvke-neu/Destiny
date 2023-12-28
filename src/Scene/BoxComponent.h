#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class RenderTargetView;
	class DepthStencilView;
	class BoxComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
		friend class RenderToTextureComponent;
	public:
		BoxComponent();
		virtual ~BoxComponent();
	public:
		void beforeDrawCommand();
		void afterDrawCommand();
		void onResize(void* data);
	public:
		GET_CLASS_NAME(BoxComponent);
	private:
		std::shared_ptr<RenderTargetView> m_renderTargetView;
		std::shared_ptr<DepthStencilView> m_depthStencilView;
	};
}