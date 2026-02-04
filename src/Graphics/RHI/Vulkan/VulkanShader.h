#pragma once
#include <vulkan/vulkan.h>

namespace Destiny
{
    class VulkanVertexShader
    {
    public:
        VulkanVertexShader(VkDevice device, VkShaderModule module)
            : m_device(device), m_shaderModule(module) {}

        ~VulkanVertexShader()
        {
            if (m_shaderModule != VK_NULL_HANDLE)
            {
                vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
            }
        }

        VkShaderModule getShaderModule() const { return m_shaderModule; }

    private:
        VkDevice m_device;
        VkShaderModule m_shaderModule;
    };

    class VulkanPixelShader
    {
    public:
        VulkanPixelShader(VkDevice device, VkShaderModule module)
            : m_device(device), m_shaderModule(module) {}

        ~VulkanPixelShader()
        {
            if (m_shaderModule != VK_NULL_HANDLE)
            {
                vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
            }
        }

        VkShaderModule getShaderModule() const { return m_shaderModule; }

    private:
        VkDevice m_device;
        VkShaderModule m_shaderModule;
    };
}
