#include "RenderSystem.h"
#if 0
#include "Visual.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "ShadowMapPipeline.h"
#include "DeferredOpaquePipeline.h"
#include "ForwardOpaquePipeline.h"
#include "TransparentPipeline.h"
#include "GuiPipeline.h"
#include "SsrPipeline.h"
#include "PostProcessingPipeline.h"
#include "BindRenderTargets.h"
#include "ClearRenderTarget.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "GpuTimer.h"
#include "Engine/EventSystem.h"
#endif

namespace Destiny
{
	RenderSystem::RenderSystem()
	{
	}

	RenderSystem::~RenderSystem()
	{
	}

	void RenderSystem::createPipeline()
	{
	}

	void RenderSystem::render()
	{
        // Bind and clear
        bindEditorRenderTarget();
        // present(); // Moved to Application::run
	}

	void RenderSystem::syncState()
	{
	}

#if 0
    // ... all other methods
	void RenderSystem::commitVisual(std::shared_ptr<Visual> visual)
	{
        // ...
	}
    // ...
#endif
}
