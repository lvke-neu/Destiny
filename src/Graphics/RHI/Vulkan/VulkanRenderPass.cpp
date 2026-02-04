#include "VulkanRenderPass.h"
#include "VulkanDevice.h"
#include <stdexcept>

namespace Destiny
{
    VulkanRenderPassCache::VulkanRenderPassCache(VulkanDevice& device)
        : m_device(device)
    {
    }

    VulkanRenderPassCache::~VulkanRenderPassCache()
    {
        for (auto& pair : m_renderPasses)
        {
            vkDestroyRenderPass(m_device.getVkDevice(), pair.second, nullptr);
        }
        m_renderPasses.clear();
    }

    VkRenderPass VulkanRenderPassCache::getRenderPass(const RenderPassKey& key)
    {
        auto it = m_renderPasses.find(key);
        if (it != m_renderPasses.end())
        {
            return it->second;
        }

        VkRenderPass renderPass = createRenderPass(key);
        m_renderPasses[key] = renderPass;
        return renderPass;
    }

    VkRenderPass VulkanRenderPassCache::createRenderPass(const RenderPassKey& key)
    {
        std::vector<VkAttachmentDescription> attachments;
        std::vector<VkAttachmentReference> colorAttachmentRefs;
        VkAttachmentReference depthAttachmentRef{};

        // Color attachments
        for (size_t i = 0; i < key.colorFormats.size(); ++i)
        {
            VkAttachmentDescription colorAttachment{};
            colorAttachment.format = key.colorFormats[i];
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = key.colorLoadOp;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = key.initialLayout; // Should match previous state
            colorAttachment.finalLayout = key.finalLayout;     // Usually COLOR_ATTACHMENT_OPTIMAL or PRESENT_SRC_KHR

            // If we LOAD, we expect it to be in COLOR_ATTACHMENT_OPTIMAL
            // If we CLEAR, the image should still be in COLOR_ATTACHMENT_OPTIMAL
            // because the barrier in VulkanSwapChain::present() transitions it there
            if (key.colorLoadOp == VK_ATTACHMENT_LOAD_OP_LOAD || key.colorLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR)
            {
                 colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 
            }

            attachments.push_back(colorAttachment);

            VkAttachmentReference ref{};
            ref.attachment = static_cast<uint32_t>(i);
            ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachmentRefs.push_back(ref);
        }

        // Depth attachment
        bool hasDepth = (key.depthFormat != VK_FORMAT_UNDEFINED);
        if (hasDepth)
        {
            VkAttachmentDescription depthAttachment{};
            depthAttachment.format = key.depthFormat;
            depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            depthAttachment.loadOp = key.depthLoadOp;
            depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            depthAttachment.stencilLoadOp = key.stencilLoadOp;
            depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            depthAttachment.initialLayout = (key.depthLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR) ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            attachments.push_back(depthAttachment);

            depthAttachmentRef.attachment = static_cast<uint32_t>(colorAttachmentRefs.size());
            depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentRefs.size());
        subpass.pColorAttachments = colorAttachmentRefs.data();
        subpass.pDepthStencilAttachment = hasDepth ? &depthAttachmentRef : nullptr;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VkRenderPass renderPass;
        if (vkCreateRenderPass(m_device.getVkDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create render pass!");
        }

        return renderPass;
    }

    // --- Framebuffer Cache ---

    VulkanFramebufferCache::VulkanFramebufferCache(VulkanDevice& device)
        : m_device(device)
    {
    }

    VulkanFramebufferCache::~VulkanFramebufferCache()
    {
        cleanup();
    }

    void VulkanFramebufferCache::cleanup()
    {
        for (auto& pair : m_framebuffers)
        {
            vkDestroyFramebuffer(m_device.getVkDevice(), pair.second, nullptr);
        }
        m_framebuffers.clear();
    }

    VkFramebuffer VulkanFramebufferCache::getFramebuffer(const FramebufferKey& key)
    {
        auto it = m_framebuffers.find(key);
        if (it != m_framebuffers.end())
        {
            return it->second;
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = key.renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(key.attachments.size());
        framebufferInfo.pAttachments = key.attachments.data();
        framebufferInfo.width = key.width;
        framebufferInfo.height = key.height;
        framebufferInfo.layers = 1;

        VkFramebuffer framebuffer;
        if (vkCreateFramebuffer(m_device.getVkDevice(), &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }

        m_framebuffers[key] = framebuffer;
        return framebuffer;
    }
}