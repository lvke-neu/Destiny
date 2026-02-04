#pragma once
#include "../GraphicsDevice.h"
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vector>

#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"

namespace Destiny
{
    struct VulkanImageView
    {
        VkImageView view;
        VkFormat format;
        uint32_t width;
        uint32_t height;
    };

    class VulkanDevice : public GraphicsDevice
    {
    public:
        VulkanDevice();
        ~VulkanDevice();

        void initialize(void* windowHandle) override;
        std::shared_ptr<GraphicsContext> getImmediateContext() override;
        std::shared_ptr<SwapChain> createSwapChain(void* windowHandle, int width, int height) override;

        VulkanPipelineCache* getPipelineCache() const { return m_pipelineCache.get(); }
        VulkanRenderPassCache* getRenderPassCache() const { return m_renderPassCache.get(); }
        VulkanFramebufferCache* getFramebufferCache() const { return m_framebufferCache.get(); }

        long CreateVertexShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppVertexShader) override;
        long CreatePixelShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppPixelShader) override;
        long CreateBuffer(const void* desc, const void* data, void** ppBuffer) override;
        void DestroyBuffer(void* buffer) override;
        long CreateTexture2D(const void* desc, const void* initialData, void** ppTexture2D) override;
        void DestroyTexture(void* texture) override;
        long CreateShaderResourceView(void* resource, const void* desc, void** ppSRView) override;
        long CreateRenderTargetView(void* resource, const void* desc, void** ppRTView) override;
        long CreateDepthStencilView(void* resource, const void* desc, void** ppDepthStencilView) override;
        long CreateInputLayout(const void* inputElementDescs, unsigned int numElements, const void* shaderBytecode, size_t shaderBytecodeLength, void** ppInputLayout) override;
        long CreateSamplerState(const void* samplerDesc, void** ppSamplerState) override;
        void DestroySamplerState(void* samplerState) override;

        long CreateRasterizerState(const void* rasterizerDesc, void** ppRasterizerState) override;
        long CreateDepthStencilState(const void* depthStencilDesc, void** ppDepthStencilState) override;
        long CreateBlendState(const void* blendStateDesc, void** ppBlendState) override;
        
        void DestroyRasterizerState(void* state) override;
        void DestroyDepthStencilState(void* state) override;
        void DestroyBlendState(void* state) override;

        VkDevice getVkDevice() const { return m_device; }
        VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }
        VkInstance getInstance() const { return m_instance; }
        VkQueue getGraphicsQueue() const { return m_graphicsQueue; }
        uint32_t getGraphicsQueueFamilyIndex() const { return m_graphicsQueueFamilyIndex; }

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        // Standard Descriptor Set Layouts
        VkDescriptorSetLayout getVSDescriptorSetLayout() const { return m_vsDescriptorSetLayout; }
        VkDescriptorSetLayout getPSDescriptorSetLayout() const { return m_psDescriptorSetLayout; }


    private:
        void createInstance();
        void pickPhysicalDevice();
        void createLogicalDevice();

        VkInstance m_instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;
        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        uint32_t m_graphicsQueueFamilyIndex = 0;

        std::shared_ptr<GraphicsContext> m_immediateContext;
        std::unique_ptr<VulkanPipelineCache> m_pipelineCache;
        std::unique_ptr<VulkanRenderPassCache> m_renderPassCache;
        std::unique_ptr<VulkanFramebufferCache> m_framebufferCache;

        // Standard Descriptor Set Layouts for resource binding
        VkDescriptorSetLayout m_vsDescriptorSetLayout = VK_NULL_HANDLE;  // Set 0: VS resources
        VkDescriptorSetLayout m_psDescriptorSetLayout = VK_NULL_HANDLE;  // Set 1: PS resources

    };
}
