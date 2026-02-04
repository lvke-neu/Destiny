#pragma once
#include "../GraphicsContext.h"
#include "VulkanDevice.h"
#include <vector>
#include <array>

namespace Destiny
{
    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(VulkanDevice* device);
        ~VulkanContext();

        void clearRenderTarget(float r, float g, float b, float a) override;
        
        // Resource Binding
        void VSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* buffers) override;
        void PSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* buffers) override;
        void PSSetShaderResources(unsigned int startSlot, unsigned int numViews, void* const* shaderResourceViews) override;
        void PSSetSamplers(unsigned int startSlot, unsigned int numSamplers, void* const* samplers) override;

        // Input Assembler
        void IASetInputLayout(void* pInputLayout) override;
        void IASetVertexBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppVertexBuffers, const unsigned int* pStrides, const unsigned int* pOffsets) override;
        void IASetIndexBuffer(void* pIndexBuffer, int format, unsigned int offset) override;
        void IASetPrimitiveTopology(int topology) override;

        // Shaders
        void VSSetShader(void* pVertexShader, void* const* ppClassInstances, unsigned int numClassInstances) override;
        void PSSetShader(void* pPixelShader, void* const* ppClassInstances, unsigned int numClassInstances) override;
        void GSSetShader(void* pGeometryShader, void* const* ppClassInstances, unsigned int numClassInstances) override {}
        void HSSetShader(void* pHullShader, void* const* ppClassInstances, unsigned int numClassInstances) override {}
        void DSSetShader(void* pDomainShader, void* const* ppClassInstances, unsigned int numClassInstances) override {}
        void CSSetShader(void* pComputeShader, void* const* ppClassInstances, unsigned int numClassInstances) override {}

        // Rasterizer
        void RSSetState(void* pRasterizerState) override;
        void RSSetViewports(unsigned int numViewports, const void* pViewports) override;
        void RSSetScissorRects(unsigned int numRects, const void* pRects) override;

        // Output Merger
        void OMSetRenderTargets(unsigned int numViews, void* const* ppRenderTargetViews, void* pDepthStencilView) override;
        void OMSetDepthStencilState(void* pDepthStencilState, unsigned int stencilRef) override;
        void OMSetBlendState(void* pBlendState, const float blendFactor[4], unsigned int sampleMask) override;

        // Draw Calls
        void DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation) override;
        
        // Temporary Stubs
        long Map(void* resource, unsigned int subresource, unsigned int mapType, unsigned int mapFlags, void* mappedResource) override { return 0; }
        void Unmap(void* resource, unsigned int subresource) override {}

        VkCommandPool getCommandPool() const { return m_commandPool; }
        VkCommandBuffer getCurrentCommandBuffer() const { return m_commandBuffer; }

        VkCommandBuffer endFrame();
        void beginFrame();
        
        // Expose for SwapChain to prepare ImGui pass
        void prepareRenderPass(); 
        void flushRenderPass(); // End current render pass if active

    private:
        VulkanDevice* m_device;
        VkCommandPool m_commandPool;
        VkCommandBuffer m_commandBuffer;

        // Render Pass State
        VkRenderPass m_currentRenderPass = VK_NULL_HANDLE;
        VkFramebuffer m_currentFramebuffer = VK_NULL_HANDLE;
        bool m_isRenderPassActive = false;
        
        // Pending Clear State
        bool m_shouldClearColor = false;
        float m_clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        bool m_shouldClearDepth = false; // Add depth clear support later if needed

        // Binding Storage
        std::array<void*, 16> m_vsConstantBuffers{};
        std::array<void*, 16> m_psConstantBuffers{};
        std::array<void*, 16> m_psShaderResources{};
        std::array<void*, 16> m_psSamplers{};

        // Pipeline State Storage
        void* m_inputLayout = nullptr;
        std::array<void*, 16> m_vertexBuffers{};
        std::array<unsigned int, 16> m_vertexStrides{};
        std::array<unsigned int, 16> m_vertexOffsets{};
        void* m_indexBuffer = nullptr;
        int m_indexFormat = 0;
        unsigned int m_indexOffset = 0;
        int m_primitiveTopology = 0;

        void* m_vertexShader = nullptr;
        void* m_pixelShader = nullptr;
        
        void* m_rasterizerState = nullptr;
        std::vector<VkViewport> m_viewports;
        std::vector<VkRect2D> m_scissorRects;
        
        std::array<void*, 8> m_renderTargetViews{};
        unsigned int m_numRenderTargets = 0;
        void* m_depthStencilView = nullptr;
        
        void* m_depthStencilState = nullptr;
        unsigned int m_stencilRef = 0;
        
        void* m_blendState = nullptr;
        float m_blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        unsigned int m_sampleMask = 0xffffffff;

        // Default States
        void* m_defaultRasterizerState = nullptr;
        void* m_defaultDepthStencilState = nullptr;
        void* m_defaultBlendState = nullptr;
    };
}
