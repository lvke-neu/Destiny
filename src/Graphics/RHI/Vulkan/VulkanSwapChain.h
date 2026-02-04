#pragma once
#include "../SwapChain.h"
#include "VulkanDevice.h"
#include "VulkanTexture.h"
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vector>

namespace Destiny
{
    class VulkanSwapChain : public SwapChain
    {
    public:
        VulkanSwapChain(VulkanDevice* device, void* windowHandle, int width, int height);
        ~VulkanSwapChain();

        void present(std::function<void(void*)> callback = nullptr) override;
        void resize(int width, int height) override;

        VkFormat getImageFormat() const { return m_swapChainImageFormat; }
        VkExtent2D getExtent() const { return m_swapChainExtent; }
        VkSwapchainKHR getHandle() const { return m_swapChain; }
        uint32_t getImageCount() const { return (uint32_t)m_swapChainImages.size(); }
        
        // RenderPass is now managed dynamically by GraphicsContext/RenderPassCache.
        // This method is kept for compatibility if needed, but should return null or be removed.
        // For now, let's remove it as we fixed the implementation.
        // If some code still calls it, we'll see errors.
        
        VulkanTexture* getBackBufferTexture() const { return m_swapChainTextures[m_currentImageIndex]; }

    private:
        void createSurface();
        void createSwapChain(int width, int height);
        void createImageViews();
        void createSyncObjects();
        void cleanupSwapChain();
        void acquireNextImage();

        VulkanDevice* m_device;
        void* m_hwnd;
        VkSurfaceKHR m_surface;
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;
        std::vector<VulkanTexture*> m_swapChainTextures;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        uint32_t m_currentImageIndex = 0;

        VkSemaphore m_imageAvailableSemaphore;
        VkSemaphore m_renderFinishedSemaphore;
        VkFence m_inFlightFence;
        bool m_imageAcquired = false;
    };
}
