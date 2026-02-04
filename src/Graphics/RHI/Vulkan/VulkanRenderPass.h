#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Destiny
{
    class VulkanDevice;

    struct RenderPassKey
    {
        std::vector<VkFormat> colorFormats;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        VkAttachmentLoadOp colorLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        VkAttachmentLoadOp depthLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        VkImageLayout finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Or PRESENT_SRC_KHR for swapchain

        bool operator==(const RenderPassKey& other) const
        {
            if (colorFormats.size() != other.colorFormats.size()) return false;
            for (size_t i = 0; i < colorFormats.size(); ++i)
            {
                if (colorFormats[i] != other.colorFormats[i]) return false;
            }
            return depthFormat == other.depthFormat &&
                   colorLoadOp == other.colorLoadOp &&
                   depthLoadOp == other.depthLoadOp &&
                   stencilLoadOp == other.stencilLoadOp &&
                   initialLayout == other.initialLayout &&
                   finalLayout == other.finalLayout;
        }
    };

    struct RenderPassKeyHash
    {
        std::size_t operator()(const RenderPassKey& k) const
        {
            std::size_t h = 0;
            auto hash_combine = [&](size_t val) {
                h ^= val + 0x9e3779b9 + (h << 6) + (h >> 2);
            };
            
            for (auto f : k.colorFormats) hash_combine(f);
            hash_combine(k.depthFormat);
            hash_combine(k.colorLoadOp);
            hash_combine(k.depthLoadOp);
            hash_combine(k.stencilLoadOp);
            hash_combine(k.initialLayout);
            hash_combine(k.finalLayout);
            return h;
        }
    };

    class VulkanRenderPassCache
    {
    public:
        VulkanRenderPassCache(VulkanDevice& device);
        ~VulkanRenderPassCache();

        VkRenderPass getRenderPass(const RenderPassKey& key);

    private:
        VkRenderPass createRenderPass(const RenderPassKey& key);

        VulkanDevice& m_device;
        std::unordered_map<RenderPassKey, VkRenderPass, RenderPassKeyHash> m_renderPasses;
    };

    struct FramebufferKey
    {
        VkRenderPass renderPass;
        std::vector<VkImageView> attachments;
        uint32_t width;
        uint32_t height;

        bool operator==(const FramebufferKey& other) const
        {
            if (renderPass != other.renderPass) return false;
            if (width != other.width || height != other.height) return false;
            if (attachments.size() != other.attachments.size()) return false;
            for (size_t i = 0; i < attachments.size(); ++i)
            {
                if (attachments[i] != other.attachments[i]) return false;
            }
            return true;
        }
    };

    struct FramebufferKeyHash
    {
        std::size_t operator()(const FramebufferKey& k) const
        {
            std::size_t h = 0;
            auto hash_combine = [&](size_t val) {
                h ^= val + 0x9e3779b9 + (h << 6) + (h >> 2);
            };
            
            hash_combine((size_t)k.renderPass);
            hash_combine(k.width);
            hash_combine(k.height);
            for (auto a : k.attachments) hash_combine((size_t)a);
            return h;
        }
    };

    class VulkanFramebufferCache
    {
    public:
        VulkanFramebufferCache(VulkanDevice& device);
        ~VulkanFramebufferCache();

        VkFramebuffer getFramebuffer(const FramebufferKey& key);
        void cleanup(); // Call periodically or on resize

    private:
        VulkanDevice& m_device;
        std::unordered_map<FramebufferKey, VkFramebuffer, FramebufferKeyHash> m_framebuffers;
    };
}