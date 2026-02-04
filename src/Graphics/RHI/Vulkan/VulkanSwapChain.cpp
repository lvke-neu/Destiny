#include "VulkanSwapChain.h"
#include "VulkanContext.h"
#include <stdexcept>
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#include <algorithm>
#include <vector>

namespace Destiny
{
    VulkanSwapChain::VulkanSwapChain(VulkanDevice* device, void* windowHandle, int width, int height)
        : m_device(device), m_hwnd(windowHandle), m_surface(VK_NULL_HANDLE), m_swapChain(VK_NULL_HANDLE),
          m_imageAvailableSemaphore(VK_NULL_HANDLE), m_renderFinishedSemaphore(VK_NULL_HANDLE), m_inFlightFence(VK_NULL_HANDLE)
    {
        createSurface();
        createSwapChain(width, height);
        createImageViews();
        createSyncObjects();
        acquireNextImage(); // Acquire first image
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        cleanupSwapChain();

        if (m_imageAvailableSemaphore)
        {
            vkDestroySemaphore(m_device->getVkDevice(), m_imageAvailableSemaphore, nullptr);
        }
        if (m_renderFinishedSemaphore)
        {
            vkDestroySemaphore(m_device->getVkDevice(), m_renderFinishedSemaphore, nullptr);
        }
        if (m_inFlightFence)
        {
            vkDestroyFence(m_device->getVkDevice(), m_inFlightFence, nullptr);
        }

        if (m_surface)
        {
            vkDestroySurfaceKHR(m_device->getInstance(), m_surface, nullptr);
        }
    }

    void VulkanSwapChain::createSurface()
    {
        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = (HWND)m_hwnd;
        createInfo.hinstance = GetModuleHandle(nullptr);

        if (vkCreateWin32SurfaceKHR(m_device->getInstance(), &createInfo, nullptr, &m_surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void VulkanSwapChain::createSwapChain(int width, int height)
    {
        VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        VkExtent2D extent = { (uint32_t)width, (uint32_t)height };

        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device->getPhysicalDevice(), m_surface, &capabilities);

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        {
            imageCount = capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_device->getVkDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(m_device->getVkDevice(), m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_device->getVkDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;
    }

    void VulkanSwapChain::createImageViews()
    {
        m_swapChainImageViews.resize(m_swapChainImages.size());
        m_swapChainTextures.resize(m_swapChainImages.size());

        for (size_t i = 0; i < m_swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_device->getVkDevice(), &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image views!");
            }
            
            m_swapChainTextures[i] = new VulkanTexture(m_device->getVkDevice(), m_swapChainImages[i], m_swapChainImageViews[i], m_swapChainExtent.width, m_swapChainExtent.height, m_swapChainImageFormat);
        }
    }

    void VulkanSwapChain::createSyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(m_device->getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(m_device->getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(m_device->getVkDevice(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create synchronization objects!");
        }
    }

    void VulkanSwapChain::acquireNextImage()
    {
        // LOG_INFO("Acquiring next image...");
        vkWaitForFences(m_device->getVkDevice(), 1, &m_inFlightFence, VK_TRUE, UINT64_MAX);
        
        VkResult result = vkAcquireNextImageKHR(m_device->getVkDevice(), m_swapChain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &m_currentImageIndex);
        // LOG_INFO("Image acquired: " + std::to_string(m_currentImageIndex));

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
             m_imageAcquired = false;
             return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        
        vkResetFences(m_device->getVkDevice(), 1, &m_inFlightFence);
        m_imageAcquired = true;
    }

    void VulkanSwapChain::cleanupSwapChain()
    {
        for (auto texture : m_swapChainTextures)
        {
            delete texture;
        }
        m_swapChainTextures.clear();

        // ImageViews are destroyed by VulkanTexture destructor because we passed ownsImage=false?
        // Wait, VulkanTexture(..., ownsImage=false) does NOT destroy the ImageView.
        // So we must destroy them here.
        for (auto imageView : m_swapChainImageViews)
        {
            vkDestroyImageView(m_device->getVkDevice(), imageView, nullptr);
        }
        m_swapChainImageViews.clear();

        if (m_swapChain)
        {
            vkDestroySwapchainKHR(m_device->getVkDevice(), m_swapChain, nullptr);
        }
    }

    void VulkanSwapChain::present(std::function<void(void*)> callback)
    {
        // LOG_INFO("Presenting frame...");
        if (!m_imageAcquired)
        {
            acquireNextImage();
            if (!m_imageAcquired) return;
        }

        auto context = std::static_pointer_cast<VulkanContext>(m_device->getImmediateContext());

        // Transition SwapChain image from UNDEFINED/PRESENT_SRC_KHR to COLOR_ATTACHMENT_OPTIMAL
        // This is necessary because after acquire, the image layout is either:
        // - UNDEFINED (first frame after creation)
        // - PRESENT_SRC_KHR (subsequent frames after presenting)
        // RenderPass expects the image in a specific initial layout for proper rendering
        {
            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Works for both UNDEFINED and PRESENT_SRC_KHR when we don't need content
            barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = m_swapChainImages[m_currentImageIndex];
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            vkCmdPipelineBarrier(
                context->getCurrentCommandBuffer(),
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );
        }

        // Get current backbuffer texture
        VulkanTexture* backBuffer = m_swapChainTextures[m_currentImageIndex];
            
        // Construct a temporary VulkanImageView struct for Context
        // Context expects void* that points to VulkanImageView
        VulkanImageView viewInfo;
        viewInfo.view = backBuffer->getImageView();
        viewInfo.format = backBuffer->getFormat();
        viewInfo.width = backBuffer->getWidth();
        viewInfo.height = backBuffer->getHeight();
            
        void* views[] = { &viewInfo };
        context->OMSetRenderTargets(1, views, nullptr);
            
        // Clear to green for debug - to confirm SwapChain is working
        context->clearRenderTarget(0.0f, 1.0f, 0.0f, 1.0f);
             
        // Ensure RenderPass is active before ImGui draws
        context->prepareRenderPass();
             
        // Execute ImGui Draw Commands
        // ImGui_ImplVulkan_RenderDrawData expects VkCommandBuffer
        if (callback)
        {
            VkCommandBuffer cmd = context->getCurrentCommandBuffer();
            callback(cmd);
        }
             
        // End RenderPass explicitly so we can insert a barrier
        context->flushRenderPass();
             
        // Clean up render targets to avoid dangling pointers
        context->OMSetRenderTargets(0, nullptr, nullptr);
             
        // Transition BackBuffer to PRESENT_SRC_KHR
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 
        barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_swapChainImages[m_currentImageIndex]; 
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
             
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = 0; 
             
        vkCmdPipelineBarrier(
            context->getCurrentCommandBuffer(),
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
        
        // 1. End Frame (Flush Context)
        VkCommandBuffer commandBuffer = context->endFrame();

        // 2. Submit
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        // LOG_INFO("Submitting queue...");
        if (vkQueueSubmit(m_device->getGraphicsQueue(), 1, &submitInfo, m_inFlightFence) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }
        
        // Force wait idle to debug deadlock/hang issues
        vkDeviceWaitIdle(m_device->getVkDevice());

        // 3. Present
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &m_currentImageIndex;

        // LOG_INFO("Queue Present...");
        VkResult result = vkQueuePresentKHR(m_device->getGraphicsQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            // resize...
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }
        
        // 4. Begin Next Frame
        acquireNextImage();
        context->beginFrame();
    }

    void VulkanSwapChain::resize(int width, int height)
    {
        // We need to ensure device is idle before destroying swapchain
        vkDeviceWaitIdle(m_device->getVkDevice());
        
        cleanupSwapChain();

        // Destroy sync objects so we can recreate them in signaled state
        // This prevents deadlock because the previous frame (before resize) was never submitted,
        // so the fence would remain unsignaled forever.
        if (m_imageAvailableSemaphore)
        {
            vkDestroySemaphore(m_device->getVkDevice(), m_imageAvailableSemaphore, nullptr);
            m_imageAvailableSemaphore = VK_NULL_HANDLE;
        }
        if (m_renderFinishedSemaphore)
        {
            vkDestroySemaphore(m_device->getVkDevice(), m_renderFinishedSemaphore, nullptr);
            m_renderFinishedSemaphore = VK_NULL_HANDLE;
        }
        if (m_inFlightFence)
        {
            vkDestroyFence(m_device->getVkDevice(), m_inFlightFence, nullptr);
            m_inFlightFence = VK_NULL_HANDLE;
        }

        createSwapChain(width, height);
        createImageViews();
        createSyncObjects();
        
        // No createFramebuffers/RenderPass needed as they are dynamic
        acquireNextImage(); // Acquire for next frame
    }
}
