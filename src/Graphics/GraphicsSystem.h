#pragma once
#include <memory>
#include <functional>
#include "RHI/GraphicsDevice.h"
#include "RHI/GraphicsContext.h"
#include "RHI/SwapChain.h"

namespace Destiny
{
	class GraphicsSystem
	{
	public:
		struct GraphicsStat
		{
			unsigned long long DrawCallCount;
			unsigned long long TriangleCount;
			unsigned long long VisualCount;
			unsigned long long DeferredVisualCount;
			unsigned long long ForwardVisualCount;
			unsigned long long TransparentVisualCount;
			unsigned long long GuiVisualCount;
		};
	public:
		GraphicsSystem();
		virtual ~GraphicsSystem();
	public:
		void						initialize(long long hwnd);
		void						uninitialize();
		void						update();
		GraphicsStat				getGraphicsStat();
		void						present(std::function<void(void*)> callback = nullptr);
	public:
		std::shared_ptr<GraphicsDevice> getDevice();
		std::shared_ptr<GraphicsContext> getImmediateContext();
		std::shared_ptr<SwapChain> getSwapChain();
	public:
		//for window resize
		void						onResize_(unsigned int width, unsigned int height);
		virtual void				syncState() = 0;
	public:
		void beginEvent(const wchar_t* name);
		void endEvent();
	public:
		void						createDeviceAndContext(void* hwnd);
		void						createSwapChain(void* hwnd);
		void						bindEditorRenderTarget();
		virtual void				createPipeline() = 0;
		virtual void				render() = 0;
	protected:
		GraphicsStat				m_graphicsStat;
		std::shared_ptr<GraphicsDevice> m_device;
		std::shared_ptr<SwapChain> m_swapChain;
	};

	inline std::shared_ptr<GraphicsDevice> GraphicsSystem::getDevice()
	{
		return m_device;
	}

	inline std::shared_ptr<GraphicsContext> GraphicsSystem::getImmediateContext()
	{
		return m_device ? m_device->getImmediateContext() : nullptr;
	}

	inline std::shared_ptr<SwapChain> GraphicsSystem::getSwapChain()
	{
		return m_swapChain;
	}

	inline GraphicsSystem::GraphicsStat GraphicsSystem::getGraphicsStat()
	{
		return m_graphicsStat;
	}
}