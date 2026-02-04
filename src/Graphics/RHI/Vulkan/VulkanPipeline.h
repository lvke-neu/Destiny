#pragma once
#include <vector>
#include <d3d11.h>
#include <unordered_map>
#include <memory>
#include <vulkan/vulkan.h>

namespace Destiny
{
    class VulkanDevice;
    class VulkanVertexShader;
    class VulkanPixelShader;

    struct VulkanRasterizerState
    {
        VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
    };

    struct VulkanDepthStencilState
    {
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    };

    struct VulkanBlendState
    {
        VkPipelineColorBlendStateCreateInfo blendInfo{};
        std::vector<VkPipelineColorBlendAttachmentState> attachments; 
    };

    class VulkanInputLayout
    {
    public:
        VulkanInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements, unsigned int numElements);
        
        std::vector<VkVertexInputBindingDescription> getBindingDescriptions() const { return m_bindings; }
        std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() const { return m_attributes; }

    private:
        std::vector<VkVertexInputBindingDescription> m_bindings;
        std::vector<VkVertexInputAttributeDescription> m_attributes;
    };

    class VulkanPipeline
    {
    public:
        VulkanPipeline(VulkanDevice& device, 
            VkPipeline pipeline, 
            VkPipelineLayout layout);
        ~VulkanPipeline();

        VkPipeline getPipeline() const { return m_pipeline; }
        VkPipelineLayout getPipelineLayout() const { return m_layout; }

        void bind(VkCommandBuffer commandBuffer);

    private:
        VulkanDevice& m_device;
        VkPipeline m_pipeline;
        VkPipelineLayout m_layout;
    };

    struct VulkanPipelineKey
    {
        VulkanVertexShader* vs;
        VulkanPixelShader* ps;
        VulkanInputLayout* inputLayout;
        VulkanRasterizerState* rasterizerState;
        VulkanDepthStencilState* depthStencilState;
        VulkanBlendState* blendState;
        VkPrimitiveTopology topology;
        VkRenderPass renderPass;
        uint32_t subpass;

        bool operator==(const VulkanPipelineKey& other) const
        {
            return vs == other.vs &&
                ps == other.ps &&
                inputLayout == other.inputLayout &&
                rasterizerState == other.rasterizerState &&
                depthStencilState == other.depthStencilState &&
                blendState == other.blendState &&
                topology == other.topology &&
                renderPass == other.renderPass &&
                subpass == other.subpass;
        }
    };

    struct VulkanPipelineKeyHash
    {
        std::size_t operator()(const VulkanPipelineKey& k) const
        {
            // Simple hash combination
            std::size_t h = 0;
            auto hash_combine = [&](size_t val) {
                h ^= val + 0x9e3779b9 + (h << 6) + (h >> 2);
            };
            
            hash_combine(std::hash<void*>{}(k.vs));
            hash_combine(std::hash<void*>{}(k.ps));
            hash_combine(std::hash<void*>{}(k.inputLayout));
            hash_combine(std::hash<void*>{}(k.rasterizerState));
            hash_combine(std::hash<void*>{}(k.depthStencilState));
            hash_combine(std::hash<void*>{}(k.blendState));
            hash_combine((size_t)k.topology);
            hash_combine((size_t)k.renderPass);
            hash_combine((size_t)k.subpass);
            return h;
        }
    };

    class VulkanPipelineCache
    {
    public:
        VulkanPipelineCache(VulkanDevice& device);
        ~VulkanPipelineCache();

        VulkanPipeline* getPipeline(const VulkanPipelineKey& key);

    private:
        VulkanPipeline* createPipeline(const VulkanPipelineKey& key);

        VulkanDevice& m_device;
        std::unordered_map<VulkanPipelineKey, std::unique_ptr<VulkanPipeline>, VulkanPipelineKeyHash> m_pipelines;
    };
}