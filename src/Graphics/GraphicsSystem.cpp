#define NOMINMAX
#include "GraphicsSystem.h"
#include "Engine/Utility.h"
#include "Math/Color.h"
#include "RHI/Vulkan/VulkanDevice.h"
#include "RHI/Vulkan/VulkanSwapChain.h"
#include <windows.h>

namespace Destiny
{
	GraphicsSystem::GraphicsSystem() :
		m_graphicsStat({ 0,0,0 })
	{

	}

	GraphicsSystem::~GraphicsSystem()
	{
	}

	void GraphicsSystem::initialize(long long hwnd)
	{
		createDeviceAndContext((void*)hwnd);
		createSwapChain((void*)hwnd);
		// onResize_(0, 0);
		createPipeline();
	}

	void GraphicsSystem::uninitialize()
	{
	}

	void GraphicsSystem::render()
	{
		// ... render loop implementation ...
	}

	void GraphicsSystem::update()
	{
		render();
		bindEditorRenderTarget();
	}

	void GraphicsSystem::onResize_(unsigned int width, unsigned int height)
	{
		if (!width || !height)
		{
			return;
		}

		if (m_swapChain)
		{
			m_swapChain->resize(width, height);
		}
	}

	void GraphicsSystem::beginEvent(const wchar_t* name)
	{
		// TODO: Vulkan debug markers
	}

	void GraphicsSystem::endEvent()
	{
		// TODO: Vulkan debug markers
	}

	void GraphicsSystem::createDeviceAndContext(void* hwnd)
	{
		m_device = std::make_shared<VulkanDevice>();
		m_device->initialize(hwnd);
	}

	void GraphicsSystem::createSwapChain(void* hwnd)
	{
		RECT rect;
		GetClientRect((HWND)hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		if (width == 0) width = 800;
		if (height == 0) height = 600;

		m_swapChain = m_device->createSwapChain(hwnd, width, height);
	}

	void GraphicsSystem::bindEditorRenderTarget()
	{
		if (m_device->getImmediateContext())
		{
			m_device->getImmediateContext()->clearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	void GraphicsSystem::present(std::function<void(void*)> callback)
	{
		if (m_swapChain)
		{
			m_swapChain->present(callback);
		}
	}
}
