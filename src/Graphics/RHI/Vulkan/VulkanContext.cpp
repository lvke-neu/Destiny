#include "VulkanContext.h"
#include "VulkanPipeline.h"
#include "VulkanShader.h"
#include "VulkanRenderPass.h"
#include "VulkanBuffer.h"
#include <d3d11.h>
#include <iostream>
#include <stdexcept>
#include "Engine/Utility.h"

namespace Destiny
{
    VulkanContext::VulkanContext(VulkanDevice* device)
        : m_device(device), m_commandPool(VK_NULL_HANDLE), m_commandBuffer(VK_NULL_HANDLE)
    {
        // Create Command Pool
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = m_device->getGraphicsQueueFamilyIndex();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(m_device->getVkDevice(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }

        // Allocate Command Buffer
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(m_device->getVkDevice(), &allocInfo, &m_commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
        
        // Start recording immediately for simplicity in this single-threaded, immediate-mode style context
        // In a real engine, we'd have BeginFrame/EndFrame
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(m_commandBuffer, &beginInfo);

        // Create Default States
        D3D11_RASTERIZER_DESC rasterDesc{};
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.CullMode = D3D11_CULL_BACK;
        rasterDesc.FrontCounterClockwise = FALSE;
        rasterDesc.DepthClipEnable = TRUE;
        m_device->CreateRasterizerState(&rasterDesc, &m_defaultRasterizerState);

        D3D11_DEPTH_STENCIL_DESC depthDesc{};
        depthDesc.DepthEnable = TRUE;
        depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
        depthDesc.StencilEnable = FALSE;
        depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        depthDesc.BackFace = depthDesc.FrontFace;
        m_device->CreateDepthStencilState(&depthDesc, &m_defaultDepthStencilState);

        D3D11_BLEND_DESC blendDesc{};
        blendDesc.AlphaToCoverageEnable = FALSE;
        blendDesc.IndependentBlendEnable = FALSE;
        blendDesc.RenderTarget[0].BlendEnable = FALSE;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        m_device->CreateBlendState(&blendDesc, &m_defaultBlendState);
    }

    VulkanContext::~VulkanContext()
    {
        if (m_device->getVkDevice() && m_commandPool)
        {
            vkDestroyCommandPool(m_device->getVkDevice(), m_commandPool, nullptr);
        }
        
        m_device->DestroyRasterizerState(m_defaultRasterizerState);
        m_device->DestroyDepthStencilState(m_defaultDepthStencilState);
        m_device->DestroyBlendState(m_defaultBlendState);
    }

    void VulkanContext::clearRenderTarget(float r, float g, float b, float a)
    {
        m_shouldClearColor = true;
        m_clearColor[0] = r;
        m_clearColor[1] = g;
        m_clearColor[2] = b;
        m_clearColor[3] = a;

        // If we are already in a render pass, we must end it so the next one can start with CLEAR op.
        // Or we could use vkCmdClearAttachments.
        // For simplicity and correctness with load ops logic, let's flush.
        flushRenderPass();
    }
    
    void VulkanContext::flushRenderPass()
    {
        if (m_isRenderPassActive)
        {
            vkCmdEndRenderPass(m_commandBuffer);
            m_isRenderPassActive = false;
            m_currentRenderPass = VK_NULL_HANDLE;
            m_currentFramebuffer = VK_NULL_HANDLE;
        }
    }

    VkCommandBuffer VulkanContext::endFrame()
    {
        flushRenderPass();
        if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
        return m_commandBuffer;
    }

    void VulkanContext::beginFrame()
    {
        vkResetCommandBuffer(m_commandBuffer, 0);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
        
        // Reset command buffer specific state tracking
        m_isRenderPassActive = false;
        m_currentRenderPass = VK_NULL_HANDLE;
        m_currentFramebuffer = VK_NULL_HANDLE;
    }

    void VulkanContext::prepareRenderPass()
    {
        if (m_numRenderTargets == 0 && !m_depthStencilView) return;

        // 1. Collect Attachments info
        RenderPassKey rpKey{};
        FramebufferKey fbKey{};
        
        std::vector<VkImageView> attachments;
        uint32_t width = 0;
        uint32_t height = 0;

        for (unsigned int i = 0; i < m_numRenderTargets; ++i)
        {
            if (m_renderTargetViews[i])
            {
                VulkanImageView* view = static_cast<VulkanImageView*>(m_renderTargetViews[i]);
                rpKey.colorFormats.push_back(view->format);
                attachments.push_back(view->view);
                
                if (width == 0) { width = view->width; height = view->height; }
            }
        }

        if (m_depthStencilView)
        {
            VulkanImageView* view = static_cast<VulkanImageView*>(m_depthStencilView);
            rpKey.depthFormat = view->format;
            attachments.push_back(view->view);
            if (width == 0) { width = view->width; height = view->height; }
        }
        
        // Determine Load Ops
        rpKey.colorLoadOp = m_shouldClearColor ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
        // rpKey.depthLoadOp = m_shouldClearDepth ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
        rpKey.depthLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // Default to load for now unless explicit ClearDepthStencilView called (todo)

        // 2. Get RenderPass
        VkRenderPass renderPass = m_device->getRenderPassCache()->getRenderPass(rpKey);
        
        // 3. Get Framebuffer
        fbKey.renderPass = renderPass;
        fbKey.attachments = attachments;
        fbKey.width = width;
        fbKey.height = height;
        
        VkFramebuffer framebuffer = m_device->getFramebufferCache()->getFramebuffer(fbKey);

        // 4. Check if we can continue current pass
        if (m_isRenderPassActive)
        {
            if (m_currentRenderPass == renderPass && m_currentFramebuffer == framebuffer && !m_shouldClearColor)
            {
                return; // Continue
            }
            flushRenderPass();
        }

        // 5. Begin RenderPass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = { width, height };

        std::vector<VkClearValue> clearValues;
        if (rpKey.colorFormats.size() > 0)
        {
             VkClearValue colorClear;
             colorClear.color = { m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3] };
             // Push one for each color attachment?
             // Currently RenderPassKey assumes same loadOp for all.
             // But we need one clear value per attachment.
             for(size_t i=0; i<rpKey.colorFormats.size(); ++i) clearValues.push_back(colorClear);
        }
        
        if (rpKey.depthFormat != VK_FORMAT_UNDEFINED)
        {
            VkClearValue depthClear;
            depthClear.depthStencil = { 1.0f, 0 };
            clearValues.push_back(depthClear);
        }

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        // Use SECONDARY_COMMAND_BUFFERS if needed, but we use INLINE for now
        vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        // Set viewport/scissor here to cover the whole RT by default, can be overridden by RSSetState/Viewports
        VkViewport viewport{};
        viewport.width = (float)width;
        viewport.height = (float)height; // Assuming normal coord system for now, flip if needed (-height)
        // Wait, Vulkan flip is y-down? No, Vulkan is y-down (0 at top). D3D is y-up? No, D3D is y-down (0 at top) for viewport?
        // D3D11 NDC is y-up (-1 bottom, 1 top). Vulkan NDC is y-down (-1 top, 1 bottom).
        // To match D3D behavior in Vulkan, we usually use negative height viewport.
        // But for Clear/ImGui, standard viewport is fine.
        // Let's stick to standard 0..width, 0..height.
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.extent = { width, height };
        vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

        m_isRenderPassActive = true;
        m_currentRenderPass = renderPass;
        m_currentFramebuffer = framebuffer;
        m_shouldClearColor = false; // Clear consumed
    }

    void VulkanContext::OMSetRenderTargets(unsigned int numViews, void* const* ppRenderTargetViews, void* pDepthStencilView)
    {
        // If targets change significantly, we might need to flush, but prepareRenderPass handles checks.
        // Just update pointers.
        m_numRenderTargets = numViews;
        for(unsigned int i=0; i<numViews; ++i)
        {
            m_renderTargetViews[i] = ppRenderTargetViews ? ppRenderTargetViews[i] : nullptr;
        }
        m_depthStencilView = pDepthStencilView;
        
        // If we change targets, we probably want to start a new pass eventually.
        // We don't force flush here because user might set targets then set other states.
        // Flush happens at Draw or Clear.
    }


    void VulkanContext::DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation)
    {
        if (!m_vertexShader || !m_pixelShader || !m_inputLayout) return;

        prepareRenderPass();

        // 1. Prepare Pipeline Key
        VulkanPipelineKey key{};
        key.vs = static_cast<VulkanVertexShader*>(m_vertexShader);
        key.ps = static_cast<VulkanPixelShader*>(m_pixelShader);
        key.inputLayout = static_cast<VulkanInputLayout*>(m_inputLayout);
        key.rasterizerState = static_cast<VulkanRasterizerState*>(m_rasterizerState ? m_rasterizerState : m_defaultRasterizerState);
        key.depthStencilState = static_cast<VulkanDepthStencilState*>(m_depthStencilState ? m_depthStencilState : m_defaultDepthStencilState);
        key.blendState = static_cast<VulkanBlendState*>(m_blendState ? m_blendState : m_defaultBlendState);
        
        switch(m_primitiveTopology) {
            case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST: key.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; break;
            case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP: key.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; break;
            case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST: key.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST; break;
            case D3D11_PRIMITIVE_TOPOLOGY_LINELIST: key.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; break;
            case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP: key.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; break;
            default: key.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; break;
        }

        key.renderPass = m_currentRenderPass;
        key.subpass = 0;
        
        VulkanPipeline* pipeline = m_device->getPipelineCache()->getPipeline(key);
        if (pipeline) pipeline->bind(m_commandBuffer);
        else return;

        // Viewports & Scissors
        if (!m_viewports.empty())
        {
            vkCmdSetViewport(m_commandBuffer, 0, static_cast<uint32_t>(m_viewports.size()), m_viewports.data());
        }
        if (!m_scissorRects.empty())
        {
            vkCmdSetScissor(m_commandBuffer, 0, static_cast<uint32_t>(m_scissorRects.size()), m_scissorRects.data());
        }
        else if (!m_viewports.empty())
        {
             VkRect2D scissor{};
             scissor.offset = {0, 0};
             scissor.extent = { (uint32_t)m_viewports[0].width, (uint32_t)std::abs(m_viewports[0].height) }; 
             vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
        }

        // Vertex Buffers
        std::vector<VkBuffer> vkVertexBuffers;
        std::vector<VkDeviceSize> vkOffsets;
        for(size_t i=0; i<16; ++i)
        {
             if(m_vertexBuffers[i])
             {
                 vkVertexBuffers.push_back(static_cast<VulkanBuffer*>(m_vertexBuffers[i])->getBuffer());
                 vkOffsets.push_back(m_vertexOffsets[i]);
             }
             else break; 
        }
        if(!vkVertexBuffers.empty())
        {
            vkCmdBindVertexBuffers(m_commandBuffer, 0, static_cast<uint32_t>(vkVertexBuffers.size()), vkVertexBuffers.data(), vkOffsets.data());
        }

        // Index Buffer
        if (m_indexBuffer)
        {
            VkIndexType indexType = (m_indexFormat == DXGI_FORMAT_R16_UINT) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
            vkCmdBindIndexBuffer(m_commandBuffer, static_cast<VulkanBuffer*>(m_indexBuffer)->getBuffer(), m_indexOffset, indexType);
        }
        
        vkCmdDrawIndexed(m_commandBuffer, indexCount, 1, startIndexLocation, baseVertexLocation, 0);
    }

    void VulkanContext::VSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* buffers)
    {
        for (unsigned int i = 0; i < numBuffers; ++i)
        {
            if (startSlot + i < m_vsConstantBuffers.size())
            {
                m_vsConstantBuffers[startSlot + i] = buffers[i];
            }
        }
    }

    void VulkanContext::PSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* buffers)
    {
        for (unsigned int i = 0; i < numBuffers; ++i)
        {
            if (startSlot + i < m_psConstantBuffers.size())
            {
                m_psConstantBuffers[startSlot + i] = buffers[i];
            }
        }
    }

    void VulkanContext::PSSetShaderResources(unsigned int startSlot, unsigned int numViews, void* const* shaderResourceViews)
    {
        for (unsigned int i = 0; i < numViews; ++i)
        {
            if (startSlot + i < m_psShaderResources.size())
            {
                m_psShaderResources[startSlot + i] = shaderResourceViews[i];
            }
        }
    }

    void VulkanContext::PSSetSamplers(unsigned int startSlot, unsigned int numSamplers, void* const* samplers)
    {
        for (unsigned int i = 0; i < numSamplers; ++i)
        {
            if (startSlot + i < m_psSamplers.size())
            {
                m_psSamplers[startSlot + i] = samplers[i];
            }
        }
    }

    void VulkanContext::IASetInputLayout(void* pInputLayout) { m_inputLayout = pInputLayout; }
    void VulkanContext::IASetVertexBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppVertexBuffers, const unsigned int* pStrides, const unsigned int* pOffsets)
    {
        for (unsigned int i = 0; i < numBuffers; ++i)
        {
            if (startSlot + i < m_vertexBuffers.size())
            {
                m_vertexBuffers[startSlot + i] = ppVertexBuffers[i];
                m_vertexStrides[startSlot + i] = pStrides[i];
                m_vertexOffsets[startSlot + i] = pOffsets[i];
            }
        }
    }
    void VulkanContext::IASetIndexBuffer(void* pIndexBuffer, int format, unsigned int offset)
    {
        m_indexBuffer = pIndexBuffer;
        m_indexFormat = format;
        m_indexOffset = offset;
    }
    void VulkanContext::IASetPrimitiveTopology(int topology) { m_primitiveTopology = topology; }

    void VulkanContext::VSSetShader(void* pVertexShader, void* const* ppClassInstances, unsigned int numClassInstances) { m_vertexShader = pVertexShader; }
    void VulkanContext::PSSetShader(void* pPixelShader, void* const* ppClassInstances, unsigned int numClassInstances) { m_pixelShader = pPixelShader; }

    void VulkanContext::RSSetState(void* pRasterizerState) { m_rasterizerState = pRasterizerState; }
    void VulkanContext::RSSetViewports(unsigned int numViewports, const void* pViewports)
    {
        const D3D11_VIEWPORT* d3dViewports = (const D3D11_VIEWPORT*)pViewports;
        m_viewports.resize(numViewports);
        for(unsigned int i=0; i<numViewports; ++i)
        {
            m_viewports[i].x = d3dViewports[i].TopLeftX;
            m_viewports[i].y = d3dViewports[i].TopLeftY + d3dViewports[i].Height; 
            m_viewports[i].width = d3dViewports[i].Width;
            m_viewports[i].height = -d3dViewports[i].Height; 
            m_viewports[i].minDepth = d3dViewports[i].MinDepth;
            m_viewports[i].maxDepth = d3dViewports[i].MaxDepth;
        }
    }
    void VulkanContext::RSSetScissorRects(unsigned int numRects, const void* pRects)
    {
        if (!pRects) return;
        const D3D11_RECT* d3dRects = (const D3D11_RECT*)pRects;
        m_scissorRects.resize(numRects);
        for(unsigned int i=0; i<numRects; ++i)
        {
            m_scissorRects[i].offset = { (int32_t)d3dRects[i].left, (int32_t)d3dRects[i].top };
            m_scissorRects[i].extent = { (uint32_t)(d3dRects[i].right - d3dRects[i].left), (uint32_t)(d3dRects[i].bottom - d3dRects[i].top) };
        }
    }

    void VulkanContext::OMSetDepthStencilState(void* pDepthStencilState, unsigned int stencilRef)
    {
        m_depthStencilState = pDepthStencilState;
        m_stencilRef = stencilRef;
    }
    void VulkanContext::OMSetBlendState(void* pBlendState, const float blendFactor[4], unsigned int sampleMask)
    {
        m_blendState = pBlendState;
        if(blendFactor) memcpy(m_blendFactor, blendFactor, sizeof(float)*4);
        m_sampleMask = sampleMask;
    }
}
